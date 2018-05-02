#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Program for the Rockem-sockem robot integrated microcontroller
// AUTHOR: Daniel Theriault
// VERSION: 0

// Based on example sketches;
// https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiWebServer/WiFiWebServer.ino
// https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiAccessPoint/WiFiAccessPoint.ino

#define COLOR "RED" // Change this when programming devices, as appropriate
#define LEFT_PIN D5
#define RIGHT_PIN D4

/* Set these to your desired credentials. */
const char *ssid = COLOR;
const char *password = "J7PYeKRFo3EG4U32JqkPSsgX";

WiFiServer server(80);

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: "); // should be 192.168.4.1
  Serial.println(myIP);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int right = 0;
  int left = 0;
  int mr;
  int ml;
  
  if (req.indexOf("/rightpunch") != -1) {
    right = 1;
    Serial.println("RIGHT PUNCH!");
  } else if (req.indexOf("/leftpunch") != -1) {
    Serial.println("LEFT PUNCH!");
    left = 1;
  } else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO
  if (right == 1) {
    digitalWrite(2, 1);
    mr = millis();
  } else if (millis() - mr >= 2000) {
    digitalWrite(2, 0);
  }

  if (left == 1) {
    digitalWrite(LEFT_PIN, 1);
    ml = millis();
  } else if (millis() - ml >= 2000) {
    digitalWrite(RIGHT_PIN, 0);
  }
  right = 0;
  left = 0;

  client.flush();

  // Prepare the response
  String s = "did stuff";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}

