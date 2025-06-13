#include <WiFi.h>        // Correct library for ESP32 WiFi
#include <HTTPClient.h>  // Correct library for ESP32 HTTPClient

// --- WiFi Credentials ---
// IMPORTANT: Replace with your actual WiFi network name and password
const char* ssid = "SSID";          // Your specific WiFi SSID
const char* password = "PASSWORD";  // Your specific WiFi Password

// --- Server Configuration ---
const char* serverUrl = "http://IP-Address:5000/api/weather";  // Your API endpoint

// Declare a WiFiClient object globally. This is required for http.begin() in newer ESP32 core versions.
WiFiClient espClient;

// --- Global Variables for Sensor Data ---
// These variables will store the latest data received from the TivaC
float currentTemperature = 0.0;
float currentHumidity = 0.0;
int currentLight = 0;
int currentPressure = 0;  // Pressure remains 0 as TivaC doesn't send it

// --- Variables for non-blocking HTTP POST timing ---
static unsigned long lastSendTime = 0;     // Stores the time of the last successful HTTP POST
const unsigned long sendInterval = 60000;  // Send interval: 60 seconds (60,000 milliseconds)

void setup() {
  // Initialize Serial communication for debugging TO your computer (via USB/Serial Monitor).
  // This uses the default USB-Serial port (GPIO1/GPIO3 on many dev boards).
  Serial.begin(115200);

  // Initialize Serial2 communication for receiving data FROM the TivaC.
  // This maps Serial2 RX to GPIO16 and TX to GPIO17.
  // Make sure your TivaC's TX is connected to ESP32's GPIO16, and TivaC's RX is connected to ESP32's GPIO17.
  Serial2.begin(115200, SERIAL_8N1, 16, 17);  // Baud rate, data bits, parity, stop bits, RX_PIN, TX_PIN

  // Small delay for initial stability
  delay(150);

  Serial.println("\n--- ESP32 Initializing ---");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);  // Initiate WiFi connection

  // Wait for WiFi to connect, printing a dot every 500ms
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");  // Newline for cleaner output
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());  // Print the IP address assigned to the ESP32

  Serial.println("Ready to receive sensor data from TivaC via UART (GPIO16/17) and send to API server.");
}

void loop() {
  // --- Section 1: Data Reception and Parsing from TivaC via UART (Serial2) ---
  // This part constantly checks if there's data available from the TivaC on Serial2.
  // It expects the human-readable string format: "Temp=25C  Humi=60%  Light=75%\r\n"
  if (Serial2.available()) {                              // Check Serial2 for incoming data
    String receivedData = Serial2.readStringUntil('\n');  // Read until a newline character from Serial2
    receivedData.trim();                                  // Remove any leading/trailing whitespace (like carriage return or extra newlines)

    Serial.print("\nReceived from TivaC: ");  // Print to the debug Serial (USB)
    Serial.println(receivedData);

    // --- Parsing logic for TivaC's specific string format ---
    // Example format: "Temp=25C  Humi=60%  Light=75%"
    int tempStart = receivedData.indexOf("Temp=") + 5;   // Find "Temp=" and move past it
    int tempEnd = receivedData.indexOf('C', tempStart);  // Find 'C' after temperature value

    int humiStart = receivedData.indexOf("Humi=", tempEnd) + 5;  // Find "Humi=" after temperature, and move past it
    int humiEnd = receivedData.indexOf('%', humiStart);          // Find '%' after humidity value

    int lightStart = receivedData.indexOf("Light=", humiEnd) + 6;  // Find "Light=" after humidity, and move past it
    int lightEnd = receivedData.indexOf('%', lightStart);          // Find '%' after light value

    // Validate that all necessary markers were found
    if (tempStart > 4 && tempEnd != -1 && humiStart > 4 && humiEnd != -1 && lightStart > 5 && lightEnd != -1) {

      // Extract substrings for each sensor value
      String tempStr = receivedData.substring(tempStart, tempEnd);
      String humiStr = receivedData.substring(humiStart, humiEnd);
      String lightStr = receivedData.substring(lightStart, lightEnd);

      // Convert parsed strings to their numerical types
      currentTemperature = tempStr.toFloat();  // Using toFloat() in case TivaC sends decimal (though it sends int)
      currentHumidity = humiStr.toFloat();     // Using toFloat() in case TivaC sends decimal (though it sends int)
      currentLight = lightStr.toInt();         // For integer light percentage

      Serial.println("Data Parsed Successfully:");
      Serial.print("   Temperature: ");
      Serial.println(currentTemperature);
      Serial.print("   Humidity: ");
      Serial.println(currentHumidity);
      Serial.print("   Light: ");
      Serial.println(currentLight);
      Serial.print("   Pressure: ");
      Serial.println(currentPressure);  // Will be 0
    } else {
      Serial.println("Error: Malformed data received from TivaC. Expected 'Temp=XXC  Humi=YY%  Light=ZZ%'");
      // Optional: Reset sensor values to default/error state if data is bad
      currentTemperature = 0.0;
      currentHumidity = 0.0;
      currentLight = 0;
      currentPressure = 0;
    }
  }

  // --- Section 2: HTTP POST Data Sending (every 60 seconds) ---
  // This section checks if it's time to send data and if WiFi is connected.
  if (WiFi.status() == WL_CONNECTED) {
    if (millis() - lastSendTime >= sendInterval) {
      lastSendTime = millis();  // Update the last send time *before* performing the HTTP request

      HTTPClient http;  // Create an HTTPClient object for the request

      // --- Format JSON Payload using the latest 'current' sensor values ---
      // This creates a JSON string like {"temperature":25.0,"humidity":60.0,"light":75,"pressure":0}
      String jsonPayload = String("{\"temperature\":") + currentTemperature + ",\"humidity\":" + currentHumidity + ",\"light\":" + currentLight + ",\"pressure\":" + currentPressure + "}";

      Serial.println("\nSending data to server...");
      Serial.println("Payload: " + jsonPayload);  // Print the JSON payload for debugging

      // --- CRITICAL FIX for "obsolete API" error ---
      // Pass the globally declared 'espClient' object to the begin method.
      http.begin(espClient, serverUrl);
      // --- END CRITICAL FIX ---

      // Set the "Content-Type" header to inform the server that we're sending JSON data
      http.addHeader("Content-Type", "application/json");

      // Send the POST request with the prepared JSON payload
      int httpCode = http.POST(jsonPayload);
      String response = http.getString();  // Get the response string from the server

      Serial.print("HTTP Code: ");
      Serial.println(httpCode);  // Print the HTTP status code (e.g., 200 for OK, -1 for connection error)
      Serial.print("Server Response: ");
      Serial.println(response);  // Print the server's response body

      http.end();  // Close the HTTP connection to free up resources

      // You might want to add more robust error handling here based on httpCode
      if (httpCode > 0) {  // HTTP status codes are positive
        // Successfully sent data
        Serial.println("Data sent successfully to server.");
      } else {
        Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    }
  } else {
    // If WiFi is disconnected, attempt to reconnect
    Serial.println("WiFi not connected! Attempting to reconnect...");
    WiFi.begin(ssid, password);
    // Give it a moment before next loop iteration, prevent rapid reconnection attempts
    delay(1000);
  }

  // Small delay to prevent the loop from running excessively fast when idle,
  // which can improve stability and overall responsiveness.
  delay(10);
}