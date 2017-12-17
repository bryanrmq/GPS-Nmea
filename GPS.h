#ifndef GPS_h
#define GPS_h
// NMEA class to parse data from GPS

#include "Arduino.h"
#include "math.h"
#include <StandardCplusplus.h>
#include <vector>
#include <iterator>

struct Location {
  double latitude;
  double longitude;
  double altitude;
  double speed;
};

struct Satellites {
  int numbers;
  int quality;
};

class GPS
{
  private:     
    Location      location;
    Satellites    satellites;
    int           timeZone = 0;
    long          time = 0;
    long          startTime = -1;
    int           signalQuality = 0;
    int           numberOfSatellites = 0;
    int           distance = 0;
    
    
    typedef int (GPS::*FunctionPointer)(std::vector<String>);
    FunctionPointer NMEATypeParserFunc[2] = {
      &GPS::parseGlobalLocation,
      &GPS::parseLocation   
    };
    
    int           parseNMEAType(String, String);
    int           parseGlobalLocation(std::vector<String>);
    int           parseLocation(std::vector<String>);
    
    String        parseTime(long);
    String        parseTime(long, bool);
    String        addPrefixToTimeNumbers(int n);
    double        _nmeaPosition2double(String, String);
    long          _nmeaTime2Time(String);
    int           _computeSpeed(double, double, double, double);
    double        _computeDistance(double, double, double, double);
    double        _deg2rad(double);
    
    static void   explode(String str, char delimiter, std::vector<String> *vector);    
	
	public:    
		GPS();
		~GPS();

		bool      encode(String);    
    bool      isValidNMEAType(String);
    
    Location  getLocation();
    int       getSignalQuality();
    int       getNumberOfSatellites();
    int       getDistance();
    
    int       getTimeZone();
    long      getTime();
    String    getParsedTime();
    long      getUpTime();
    String    getParsedUpTime();

    void      setTimezone(int hours);        
};

#endif
