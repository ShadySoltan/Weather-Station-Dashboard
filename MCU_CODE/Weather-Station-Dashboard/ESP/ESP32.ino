// ESP32 UART Bridge: UART <-> USB Serial
// Uses UART2 pins by default (GPIO16 RX2, GPIO17 TX2)

HardwareSerial MySerial(2);  // Use UART2

void setup() {
  Serial.begin(115200);        // USB serial for PC
  MySerial.begin(115200, SERIAL_8N1, 16, 17); // UART2, baud 115200, RX=GPIO16, TX=GPIO17
}

void loop() {
  // Forward data from UART to USB Serial
  if (MySerial.available()) {
    int c = MySerial.read();
    Serial.write(c);
  }

  // Forward data from USB Serial to UART
  if (Serial.available()) {
    int c = Serial.read();
    MySerial.write(c);
  }
}




// void setup() {
//   // Initialize Serial2 (pre-defined by ESP32 core)
//   Serial2.begin(115200);
//   Serial2.setPins(16, 17);  // Optional: Customize pins

//   // Debug output
//   Serial.begin(115200);
//   Serial.println("ESP32 UART Ready!");
// }

// void loop() {
//   if (Serial2.available()) {
//     String data = Serial2.readStringUntil('\n');
//     if (data.length() > 0 && data[0] != 'X') {  // Filter garbage
//       Serial.print("[Valid Data] ");
//       Serial.println(data);
//     } else {
//       Serial.println("[Error] Garbled data received!");
//     }
//   }
// }