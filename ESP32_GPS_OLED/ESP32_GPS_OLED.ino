#include <TinyGPSPlus.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// #include <WiFi.h> j
#include <WiFiClientSecure.h>
#include <HTTPSRedirect.h>
#include <HTTPClient.h>

const char* ssid = "Huawei-Huawei";
const char* password = "255512hg";

const char* serverName = "https://location-live-tracking-muf7kziviq-as.a.run.app/receive_data";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// const char* test_root_ca = \"-----BEGIN CERTIFICATE-----\n"
//                       "MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw\n" \
//                       "CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n" \
//                       "MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n" \
//                       "MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n" \
//                       "Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA\n" \
//                       "A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo\n" \
//                       "27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w\n" \
//                       "Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw\n" \
//                       "TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl\n" \
//                       "qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH\n" \
//                       "szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8\n" \
//                       "Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk\n" \
//                       "MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92\n" \
//                       "wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p\n" \
//                       "aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN\n" \
//                       "VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID\n" \
//                       "AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n" \
//                       "FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb\n" \
//                       "C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe\n" \
//                       "QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy\n" \
//                       "h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4\n" \
//                       "7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J\n" \
//                       "ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef\n" \
//                       "MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/\n" \
//                       "Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT\n" \
//                       "6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ\n" \
//                       "0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm\n" \
//                       "2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb\n" \
//                       "bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n" \
//                       "-----END CERTIFICATE-----\n";

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// The TinyGPSPlus object
TinyGPSPlus gps;

WiFiClientSecure client;
// client.setInsecure();

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(3000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  client.setInsecure();

}


void loop() {
  // updateSerial();

  static unsigned long lastUpdate = 0;
  unsigned long currentMillis = millis();
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      // displayInfo();
      // print_speed();

      if (currentMillis - lastUpdate >= 1000) { // Update every 1000 milliseconds (1 second)
      lastUpdate = currentMillis;

      // Call your functions that need to be updated at 1Hz here
      displayInfo();
      print_speed();
      
      }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void displayInfo() {


  Serial.print(F("Location: ")); 
  // String text = "Location: ";
  if (gps.location.isValid())
  {
    send_data();
    Serial.print(gps.location.lat(), 6);
    // text += gps.location.lat();
    Serial.print(F(","));
    // text += ",";
    Serial.print(gps.location.lng(), 6);
    // text += gps.location.lng();
  }
  else
  {
    Serial.print(F("INVALID"));
    // text += "INVALID";
  }
  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
    // text += "INVALID";
  }
  Serial.print(F(" "));
  // text += " ";
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    // text+= gps.time.hour();
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    // text+= gps.time.minute();
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    // Serial.println(gps.time.)
    // text+= gps.time.second();
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
    // text+= "INVALID";
  }
  // display.clearDisplay();
  Serial.println();
  // display.println(text);
  // display.display();
}

void print_speed()
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
       
  if (gps.location.isValid() == 1)
  {
   //String gps_speed = String(gps.speed.kmph());
    display.setTextSize(1);
    
    display.setCursor(25, 5);
    display.print("Lat: ");
    display.setCursor(50, 5);
    display.print(gps.location.lat(),6);
 
    display.setCursor(25, 20);
    display.print("Lng: ");
    display.setCursor(50, 20);
    display.print(gps.location.lng(),6);
 
    display.setCursor(25, 35);
    display.print("Speed: ");
    display.setCursor(65, 35);
    display.print(gps.speed.kmph());
    
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("SAT:");
    display.setCursor(25, 50);
    display.print(gps.satellites.value());
 
    display.setTextSize(1);
    display.setCursor(70, 50);
    display.print("ALT:");
    display.setCursor(95, 50);
    display.print(gps.altitude.meters(), 0);
 
    display.display();
    
  }
  else
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("No Data");
    display.display();
  }  
 
}

void send_data() {
  if (WiFi.status() == WL_CONNECTED) { // Check if connected to Wi-Fi
    HTTPClient http; // Create an HTTPClient object
    http.begin(client, serverName); // Initialize with the server URL and the secure client
    http.addHeader("Content-Type", "application/json"); // Set the content type to JSON
    http.addHeader("Authorization", "Bearer secret"); // Set the authorization header

    // Create the JSON object with the GPS data
    String postData = "{\"tracker_id\": 2, \"lat\": " + String(gps.location.lat(), 6) + ", \"lon\": " + String(gps.location.lng(), 6) + "}";



    int httpResponseCode = http.POST(postData); // Send the POST request and get the response code

    if (httpResponseCode > 0) { // Check if the request was successful
      String response = http.getString(); // Get the response payload
      Serial.println("HTTP Response code: " + String(httpResponseCode)); // Print the response code
      Serial.println("Response: " + response); // Print the response payload
    } else {
      Serial.println("Error in sending HTTP POST request: " + String(httpResponseCode)); // Print the error code
    }

    http.end(); // Close the connection
  } else {
    Serial.println("Wi-Fi is not connected");
  }
}


void updateSerial() {
  delay(500);
  // while (Serial.available()) {
  //   Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  // }
  // while (Serial2.available())
  // {
  //   Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  //   // Serial.print(char(Serial2.read()));
  // }

    while (Serial2.available()) {
      Serial.print(char(Serial2.read()));
    }


}