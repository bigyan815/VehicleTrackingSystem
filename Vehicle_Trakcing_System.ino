#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
static const int RXPin = 4, TXPin = 5;  
static const uint32_t GPSBaud = 9600; 
TinyGPSPlus gps;
WidgetMap myMap(V0);  
SoftwareSerial ss(RXPin, TXPin);  
BlynkTimer timer;
float spd;       
float sats;      
String bearing;  
char auth[] = "XIIpHMe8qTXOy79ZtDocJD2LcsVx5R1O";
char ssid[] = "anamolous_2.4";
char pass[] = "nepal@123";                                         
//unsigned int move_index;         
unsigned int move_index = 1;       
void setup()
{
  Serial.begin(115200);
  Serial.println();
  ss.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, checkGPS); // every 5s check if GPS is connected, only really needs to be done once
}
void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
      Blynk.virtualWrite(V4, "GPS ERROR");  // Value Display widget  on V4 if GPS not detected
  }
}
void loop()
{
    while (ss.available() > 0) 
    {
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(ss.read()))
        displayInfo();
  }
  Blynk.run();
  timer.run();
}
void displayInfo()
{ 
  if (gps.location.isValid() ) 
  {    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng()); 
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V1, String(latitude, 6));   
    Blynk.virtualWrite(V2, String(longitude, 6));  
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    spd = gps.speed.kmph();               //get speed
       Blynk.virtualWrite(V3, spd);
       sats = gps.satellites.value();    //get number of satellites
       Blynk.virtualWrite(V4, sats);
       bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
       Blynk.virtualWrite(V5, bearing);                   
  }
 Serial.println();
}
