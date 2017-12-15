#include "GPS.h"

GPS gps;
String str;

void setup() {
  gps.setTimezone(1);
  
  // initialize both serial ports:
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  // read from port 1, send to port 0:
  if (Serial1.available()) {
    String str = Serial1.readStringUntil('\n');
    if(gps.encode(str)) {
      Serial.println("----------");
      Serial.println(gps.getLocation().latitude);
      Serial.println(gps.getLocation().longitude);
      Serial.println(gps.getLocation().altitude);
      Serial.println(gps.getLocation().speed);
      Serial.println(gps.getSignalQuality());
      Serial.println(gps.getNumberOfSatellites());
      Serial.println(gps.getParsedTime());
      Serial.println(gps.getUpTime());
      Serial.println(gps.getParsedUpTime());
      Serial.println("----------");
    }
  }
}
