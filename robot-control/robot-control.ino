#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Program for the Rockem-sockem robot integrated microcontroller
// AUTHOR: Daniel Theriault
// VERSION: 0

// Based on example sketches;
// https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiWebServer/WiFiWebServer.ino
// https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiAccessPoint/WiFiAccessPoint.ino

#define COLOR "RED" // Change this when programming devices, as appropriate
#define LEFT_PIN D2
#define RIGHT_PIN D1

/* Set these to your desired credentials. */
const char *ssid = COLOR;
const char *password = "J7PYeKRFo3EG4U32JqkPSsgX";

WiFiUDP Udp;
unsigned int port = 3651;  // local port to listen on
char recvbuf[1];

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
}

void loop() {
  int packetSize = Udp.parsePacket();
  
  if (packetSize) {
    // receive incoming UDP packets
    int len = Udp.read(recvbuf, 255);
    if (len > 0) {
      recvbuf[len] = 0;
    }
    
//    Serial.print("Received packet: ");
//    Serial.print(recvbuf[0]);
//    Serial.print(" (Size: ");
//    Serial.print(packetSize);
//    Serial.println(")");

    if (recvbuf[0] == 'R') {
      Serial.println("RIGHT PUNCH!");
      digitalWrite(RIGHT_PIN, HIGH);
      delay(2000);
      digitalWrite(RIGHT_PIN, LOW);
    } else if (recvbuf[0] == 'L') {
      Serial.println("LEFT PUNCH!");
      digitalWrite(LEFT_PIN, HIGH);
      delay(2000);
      digitalWrite(LEFT_PIN, LOW);
    }
  }
 
  

}

