#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

// Program for the Rockem-sockem robot integrated microcontroller
// AUTHOR: Daniel Theriault
// VERSION: 0

// Based on example sketches;
// https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiWebServer/WiFiWebServer.ino
// https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiAccessPoint/WiFiAccessPoint.ino
// https://www.arduino.cc/en/Tutorial/Sweep

#define COLOR "BLUE" // Change this when programming devices, as appropriate
#define LEFT_PIN D1
#define RIGHT_PIN D2
#define PUNCH_ANGLE 120


/* Set these to your desired credentials. */
const char *ssid = COLOR;
const char *password = "J7PYeKRFo3EG4U32JqkPSsgX";

WiFiUDP Udp;
unsigned int port = 3651;  // local port to listen on
char recvbuf[1];

Servo lServo;
Servo rServo;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: "); // should be 192.168.4.1
  Serial.println(myIP);
  
  Udp.begin(port);

  pinMode(LEFT_PIN, OUTPUT);
  pinMode(RIGHT_PIN, OUTPUT);

  lServo.attach(LEFT_PIN);
  lServo.write(0);
  rServo.attach(RIGHT_PIN);
  rServo.write(180);
}

void loop() {
  int packetSize = Udp.parsePacket();
  
  if (packetSize) {
    
    // receive incoming UDP packets
    int len = Udp.read(recvbuf, 255);
    if (len > 0) {
      recvbuf[len] = 0;
    }

    // control the servos in response
    if (recvbuf[0] == 'R') {
      Serial.println("RIGHT PUNCH!");
      
      rServo.write(180 - PUNCH_ANGLE);
      delay(600);
      rServo.write(180);
      
    } else if (recvbuf[0] == 'L') {
      Serial.println("LEFT PUNCH!");

      lServo.write(PUNCH_ANGLE);
      delay(600);
      lServo.write(0);
    }
  }
}

