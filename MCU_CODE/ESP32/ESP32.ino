#include <WiFi.h>           // For ESP32 WiFi functions
#include <HTTPClient.h>     // For ESP32 HTTP client
#include <WebServer.h>      // For ESP32 WebServer (used by WiFiManager)
#include <DNSServer.h>      // Required by WiFiManager (same for ESP32)
#include <WiFiManager.h>    // The WiFiManager library (should be compatible with ESP32)

// --- Server Configuration ---
const char* serverUrl = "http://localhost:5000/api/weather"; // Your API endpoint

// Declare a WiFiClient object globally.
WiFiClient espClient;

// --- Global Variables for Sensor Data ---
float currentTemperature = 0.0;
float currentHumidity = 0.0;
int currentLight = 0;
int currentPressure = 0; // Pressure remains 0 as TivaC doesn't send it

void setup() {
  Serial.begin(115200); // TivaC communication baud rate

  delay(150);
  Serial.println("\n--- ESP32 Initializing ---"); // Updated message for ESP32

  // --- WiFiManager Setup ---
  WiFiManager wifiManager;

  // Optional: Uncomment to reset saved settings for testing
  // wifiManager.resetSettings(); // ONLY UNCOMMENT FOR TESTING, THEN RECOMMENT!

  // --- CUSTOMIZATION FOR THE HEADER AND BACKGROUND ---
  // THIS IS THE CRITICAL STRING.
  // PASTE YOUR FULL BASE64 ENCODED IMAGE DATA HERE, EXACTLY BETWEEN THE SINGLE QUOTES.
  // IT MUST START WITH 'data:image/jpeg;base64,' AND CONTAIN NO EXTRA SPACES/NEWLINES.
  const char* customHeaderHtmlWithScript = R"raw(<style>
    /* Existing CSS for H1 header hiding and styling */
    h1 { display: none; } /* Hide the default H1 (WiFiManager text) */
    .wrap-h1 { text-align: center; } /* Center the wrapper if it exists */
    #custom-project-header {
      font-family: Arial, sans-serif; /* You can choose your font */
      font-size: 28px; /* Adjust font size as needed */
      color: #337ab7; /* A nice blue, similar to buttons */
      margin-top: 20px;
      margin-bottom: 10px;
      font-weight: bold;
    }
    .wifi-manager-header { /* Targeting the specific class if it's there */
        display: none;
    }

    /* NEW CSS FOR BACKGROUND IMAGE */
    body {
      background-image: url('YOUR_COMPLETE_BASE64_STRING_GOES_HERE_INCLUDING_DATA_IMAGE_JPEG_BASE64_PREFIX');
      background-size: cover;       /* Scales the image to cover the entire background area */
      background-repeat: no-repeat; /* Prevents the image from repeating */
      background-position: center center; /* Centers the image */
      background-attachment: fixed; /* Keeps the background fixed when content scrolls */
      /* You might also need to adjust text colors for readability against your background */
      color: #333; /* Default text color for the page, adjust as needed */
    }
  </style>
  <h1 id="custom-project-header" style="display:none;">Weather Dashboard Station</h1>
  <script>
    window.onload = function() {
      var header = document.getElementById('custom-project-header');
      if (header) {
        header.style.display = 'block'; // Make your custom header visible
        var body = document.querySelector('body'); // Get the page body
        if (body) {
          // Insert your custom header as the very first child of the body
          body.insertBefore(header, body.firstChild);
        }
      }
    };
  </script>)raw";

  wifiManager.setCustomHeadElement(customHeaderHtmlWithScript);
  // --- END CUSTOMIZATION ---

  // --- CUSTOMIZATION TO REMOVE BUTTONS ---
  const char* menuItems[] = {"wifi", "exit"};
  wifiManager.setMenu(menuItems, 2);
  // --- END CUSTOMIZATION ---


  Serial.println("Attempting to connect to stored WiFi or start AP for configuration...");
  if (!wifiManager.autoConnect("Weather_dashboard_Connection", "password")) {
    Serial.println("Failed to connect and timed out.");
    ESP.restart(); // Restart and try again
    delay(5000);
  }

  Serial.println("\nWiFi connected!");
  Serial.print("ESP32 IP Address: "); // Updated message for ESP32
  Serial.println(WiFi.localIP());

  Serial.println("Ready to receive sensor data from TivaC via UART and send to API server.");
}

void loop() {
  // --- Section 1: Data Reception and Parsing from TivaC via UART ---
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    receivedData.trim();

    Serial.print("\nReceived from TivaC: ");
    Serial.println(receivedData);

    // --- Parsing logic for TivaC's specific string format ---
    int tempStart = receivedData.indexOf("Temp=") + 5;
    int tempEnd = receivedData.indexOf('C', tempStart);

    int humiStart = receivedData.indexOf("Humi=", tempEnd) + 5;
    int humiEnd = receivedData.indexOf('%', humiStart);

    int lightStart = receivedData.indexOf("Light=", humiEnd) + 6;
    int lightEnd = receivedData.indexOf('%', lightStart);

    if (tempStart > 4 && tempEnd != -1 &&
        humiStart > 4 && humiEnd != -1 &&
        lightStart > 5 && lightEnd != -1) {

      String tempStr = receivedData.substring(tempStart, tempEnd);
      String humiStr = receivedData.substring(humiStart, humiEnd);
      String lightStr = receivedData.substring(lightStart, lightEnd);

      currentTemperature = tempStr.toFloat();
      currentHumidity = humiStr.toFloat();
      currentLight = lightStr.toInt();

      Serial.println("Data Parsed Successfully:");
      Serial.print("  Temperature: "); Serial.println(currentTemperature);
      Serial.print("  Humidity: "); Serial.println(currentHumidity);
      Serial.print("  Light: "); Serial.println(currentLight);
      Serial.print("  Pressure: "); Serial.println(currentPressure); // Will be 0

      // --- Section 2: HTTP POST Data Sending (IMMEDIATE) ---
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        String jsonPayload = String("{\"temperature\":") + currentTemperature +
                             ",\"humidity\":" + currentHumidity +
                             ",\"light\":" + currentLight +
                             ",\"pressure\":" + currentPressure + "}";

        Serial.println("\nSending data to server...");
        Serial.println("Payload: " + jsonPayload);

        http.begin(espClient, serverUrl);
        http.addHeader("Content-Type", "application/json");

        int httpCode = http.POST(jsonPayload);
        String response = http.getString();

        Serial.print("HTTP Code: ");
        Serial.println(httpCode);
        Serial.print("Server Response: ");
        Serial.println(response);

        http.end();

        if (httpCode > 0) {
          Serial.println("Data sent successfully to server.");
        } else {
          Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
      } else {
        Serial.println("WiFi not connected! Cannot send data. Attempting to reconnect...");
      }

    } else {
      Serial.println("Error: Malformed data received from TivaC. Expected 'Temp=XXC  Humi=YY%  Light=ZZ%'");
      currentTemperature = 0.0;
      currentHumidity = 0.0;
      currentLight = 0;
      currentPressure = 0;
    }
  }

  // Small delay for stability
  delay(10);
}