/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.
    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Program for Rockem-sockem robot remote control
// AUTHOR: Daniel Theriault
// VERSION: 0

#define COLOR "RED"
#define LEFT_PIN D5
#define RIGHT_PIN D4

const char* ssid     =  COLOR;
const char* password = "J7PYeKRFo3EG4U32JqkPSsgX";

const char* host = "192.168.4.1";
WiFiUDP Udp;
unsigned int port = 3651;
char recv[1];

// used for oneshot behavior
int leftshot = 0;
int rightshot = 0;

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

  Udp.begin(port);
}

int value = 0;

void loop() {
  if (digitalRead(LEFT_PIN) == 1) {
    if (leftshot == 1) return;
    Serial.println("LEFT!");
    Udp.beginPacket(host, port);
    Udp.write('L');
    Udp.endPacket();
    delay(400);
  } else {
    leftshot == 0;
  }
  
  if (digitalRead(RIGHT_PIN) == 1) {
    if (rightshot == 1) return;
    Serial.println("RIGHT!");
    Udp.beginPacket(host, port);
    Udp.write('R');
    Udp.endPacket();
    delay(400);
  } else {
    rightshot == 0;
  }
}

