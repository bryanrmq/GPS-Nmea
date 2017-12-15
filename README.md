# GPS-Nmea

This library is a simple NMEA parser that allow you to use your GPS shild easely. I used this library with a uBlox NEO-7M GPS.

This library allows you to quickly know your location, altitude, speed (km/h), signal quality, number of satellites, UTC time and the up time.

> Note: At this point GPS-Nmea only parse the GPGGA instruction.

# How to use

> This example is made for Arduino Mega with Serial & Serial 1
> Note: I put the GPS module on RX1 & TX1

```
#include "GPS.h"

GPS gps;

//...

void setup() {
	// set the timezone +1 for Paris/Europe
	gps.setTimezone(1);

	Serial.begin(9600);
	Serial1.begin(9600);
}

void loop() {
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
```

# How do I install it?

This is installed just like a regular Arduino library. Import the .zip inside the Arduino App and it will do the trick.

Be sure to reset your Arduino IDE after installing it.

# Dependencies

Note: at this point GPS-Nmea requires the [StandardCPlusPlus](https://github.com/maniacbug/StandardCplusplus) library to work properly.

# What is the license?

This library is under MIT License.