/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.
    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.
*/

#include <ESP8266WiFi.h>

// Program for Rockem-sockem robot remote control
// AUTHOR: Daniel Theriault
// VERSION: 0

#define COLOR "RED"
#define LEFT_PIN D5
#define RIGHT_PIN D4

const char* ssid     =  COLOR;
const char* password = "J7PYeKRFo3EG4U32JqkPSsgX";

const char* host = "192.168.4.1";
const char* streamId   = "....................";
const char* privateKey = "....................";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH);
  pinMode(LEFT_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
}

int value = 0;

void loop() {
  String url;
  Serial.print("left: ");
  Serial.print(digitalRead(LEFT_PIN));
  Serial.print(", right: ");
  Serial.println(digitalRead(RIGHT_PIN));
  if (digitalRead(LEFT_PIN) == 1) {
    url = "/leftpunch/";
    Serial.println("stiff left cut!");
  } else if (digitalRead(RIGHT_PIN) == 1) {
    url = "/rightpunch/";
    Serial.println("hard right hook!");
  } else {
    return;
  }
  
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}

