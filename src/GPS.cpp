#include "Arduino.h"
#include "GPS.h"

static const String NMEATypes[] = { 
  "GPGGA", 
//  "GPGLL" 
};

GPS::GPS() {
  
}

GPS::~GPS() {
  
}

bool GPS::encode(String str) {
  if(str.length() > 5) {
    String NMEAType = str.substring(1, 6);
    if(this->isValidNMEAType(NMEAType)) {
      String NMEAData = str.substring(7);
      if (this->parseNMEAType(NMEAType, NMEAData) == -1)
        return false;
      return true;
    }
  }
  return false;
}

bool GPS::isValidNMEAType(String type) {
  for(int i = 0; i < sizeof(NMEATypes); i++) {
    if (NMEATypes[i] == type)
      return true;
  }
  return false;
}




// Getters
Location GPS::getLocation() {
  return this->location;
}

int GPS::getSignalQuality() {
  return this->signalQuality; 
}

int GPS::getNumberOfSatellites() {
  return this->numberOfSatellites;   
}

int GPS::getTimeZone() {
  return this->timeZone;   
}

long GPS::getTime() {
  return this->time;
}

String GPS::getParsedTime() {
  return this->parseTime(this->time);
}

String GPS::parseTime(long time) {
  return this->parseTime(time, true);
}

String GPS::parseTime(long time, bool show) {
  int seconds = 0, minutes = 0, hours = 0;
  String str = this->addPrefixToTimeNumbers(time % 60);
  if ((minutes = ((time - seconds) % (60 * 60)) / 60) > 0 || show)
    str = this->addPrefixToTimeNumbers(minutes) + ":" + str;
  if ((hours = (time - minutes) / 60 / 60) > 0 || show)
    str = this->addPrefixToTimeNumbers(hours) + ":" + str;
  return str;
}

long GPS::getUpTime() {
  return this->time - this->startTime;
}

String GPS::getParsedUpTime() {
  return this->parseTime(this->getUpTime(), false);
}

String GPS::addPrefixToTimeNumbers(int n) {
  return (n < 10) ? "0" + String(n) : String(n);
}


// Setters
void GPS::setTimezone(int hours) {
  this->timeZone = hours;
}



// Core
int GPS::parseNMEAType(String type, String data) {
  for(int i = 0; i < sizeof(NMEATypes); i++) {
    if (NMEATypes[i] == type) {
      std::vector<String> datas;
      explode(data, ',', &datas);
      return (this->*NMEATypeParserFunc[i])(datas);
    }
  }
  return -1;
}

int GPS::parseGlobalLocation(std::vector<String> datas) {
  double lat = this->location.latitude;
  double lng = this->location.longitude;
  
  this->location.latitude = this->_nmeaPosition2double(datas[1], datas[2]);
  this->location.longitude = this->_nmeaPosition2double(datas[3], datas[4]);
  this->location.altitude = datas[8].toDouble();
  this->signalQuality = datas[5].toInt();
  this->numberOfSatellites = datas[6].toInt();
  this->time = this->_nmeaTime2Time(datas[0]);
  if(this->startTime == -1)
    this->startTime = this->_nmeaTime2Time(datas[0]);

  this->location.speed = this->_computeSpeed(lat, lng, this->location.latitude, this->location.longitude);
  
  return 1;
}

int GPS::parseLocation(std::vector<String> datas) {
  this->location.latitude = this->_nmeaPosition2double(datas[0], datas[1]);
  this->location.longitude = this->_nmeaPosition2double(datas[2], datas[3]);
  this->time = this->_nmeaTime2Time(datas[4]);
  if(this->startTime == -1)
    this->startTime = this->_nmeaTime2Time(datas[4]);
  return 1;
}




// Tools
int GPS::_computeSpeed(double lat1, double lng1, double lat2, double lng2) {
  int speed = (int) round(this->_computeDistance(lat1, lng1, lat2, lng2) * 3.6);
  return (speed < 5) ? 0 : speed;
}

double GPS::_computeDistance(double lat1, double lng1, double lat2, double lng2) {
  double earthRadius = 6371000.;
  double distLat = this->_deg2rad(lat2 - lat1);
  double distLng = this->_deg2rad(lng2 - lng1);
  double a = pow(sin(distLat / 2), 2) + cos(this->_deg2rad(lat1)) * cos(this->_deg2rad(lat2)) * pow(sin(distLng / 2), 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return earthRadius * c;
}

double GPS::_deg2rad(double degrees) {
  return degrees * (M_PI / 180.);
}

double GPS::_nmeaPosition2double(String position, String direction) {
  if(position.length() <= 2)
    return 0.00;
  double degrees = position.substring(0, position.indexOf(".") - 2).toDouble();
  if(degrees < 0)
    return 0.00;
  double minutes = position.substring(position.indexOf(".") - 2).toDouble();
  double pos = degrees + (minutes / 60.);
  if (direction == "W" || direction == "S")
    return -1 * pos;
  return pos;
}

long GPS::_nmeaTime2Time(String time) {
  return ((time.substring(0, 2).toInt() + this->timeZone) * 60 * 60) + (time.substring(2, 4).toInt() * 60) + (time.substring(4, 6).toInt());
}

static void GPS::explode(String str, char delimiter, std::vector<String> *vector) {
  char* temp;
  char str2[str.length()];
  str.toCharArray(str2, str.length());
  temp = strchr(str2, ',');
  int startPosition = 0;
  while(temp != NULL) {
    vector->push_back(String(str2).substring(startPosition, temp - str2));
    startPosition = temp - str2 + 1;
    temp = strchr(temp + 1, ',');
  }
  // last part
  vector->push_back(String(str2).substring(startPosition));
}

