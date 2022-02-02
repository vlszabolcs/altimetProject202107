// Create a Car class with some attributes
//#include <BluetoothSerial.h>
//BluetoothSerial SerialBT;

#include <TinyGPS++.h>
TinyGPSPlus gps;
#define RXD2 16
#define TXD2 17

#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;


#include "SD.h"

/*
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
*/

class altiFunc {
    public:
        String message;
        String path;

        double gpsLon;
        double gpsLat;
        double gpsAlti;
        double gpsSpd;
        int    gpsDate;
        int    gpsTime;


        char c ;
        File file;
        float pres;
        float temp ;
        float presAlti ;
        float presCorrig;

        void sensData();
        void gpsData();
       
        void logCSV();

        //void valueDISP(String acutalMenu);
    private:
       bool recFirstRun=true;
       bool status = true;
       const int period= 1000;
       unsigned long time_now = 0;

       /*const char time=     'Time     :';
       const char lat=      'Lat      :';
       const char lon=      'Lon      :';
       const char gpsAlt =  'GPSAlti  :';
       const char spd =     'Speed    :';
       const char presS =    'Pres     :';
       const char tmepS =    'Temp     :';
       const char prsAltS =  'presAlti :';*/ 
};

void altiFunc::sensData () {
  
  pres = bmp.readPressure() / 100.0F;
  temp = bmp.readTemperature()-2;
  presAlti = bmp.readAltitude(presCorrig);
  
}

void altiFunc::gpsData () {

  // This sketch displays information every time a new sentence is correctly encoded.
    
   if (Serial2.available()){
        c = Serial2.read();
        //Serial.write(c);
        gps.encode(c);
    }
    
    
      gpsAlti = gps.altitude.meters();
      gpsLon =  gps.location.lat();
      gpsLat =  gps.location.lng();
      gpsSpd =  gps.speed.kmph(); 
      gpsDate = gps.date.value();
      gpsTime = gps.time.value();

  /*  if(millis() > time_now + period){
      time_now = millis();
  
      gpsAlti =gps.altitude.meters();
      gpsLon = gps.location.lat();
      gpsLat = gps.location.lng();
      gpsSpd = gps.speed.kmph(); 
      gpsTime = String(gps.date.value());
    
      Serial.println();
      Serial.print("Altitude: ");
      Serial.println(String(gpsAlti, 6));
      Serial.print("Longtitude: ");
      Serial.println(gps.location.lng(), 6);
      Serial.print("Latitude: ");
      Serial.println(gpsLat);
      Serial.print("Date: ");
      Serial.print(String(gps.date.value()));
      Serial.print(" ");
      Serial.println(String(gps.time.value()));
        
      if (gps.time.hour() < 10) Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(":");
      if (gps.time.minute() < 10) Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(":");
      if (gps.time.second() < 10) Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(".");
      if (gps.time.centisecond() < 10) Serial.print(F("0"));
        Serial.println(gps.time.centisecond());
        Serial.println();
    }*/
}



void altiFunc:: logCSV(){
    sensData(); 

    message=String(gpsDate) + "_" +
            String(gpsTime) + "," + 
            String(gpsLat,6)  + "," +
            String(gpsLon,6)  + "," +
            String(gpsAlti,6) + "," +
            String(gpsSpd)  + "," + 
            String(pres)    + "," + 
            String(temp)    + "," + 
            String(presAlti);

    if (recFirstRun == true) {
       ///creat file name 
       if (gps.time.isValid()){
         
        String filename = String(gps.date.value())+"_"+String(gps.time.value());
        path = "/" + filename + ".csv";
        String head = "Time,Latitude,Longitude ,GPSAltitude,Speed,Pressure,Temperature,Altitude";
        //Serial.print("RecordMenu--> FirstRun: ");
        Serial.println(path);
        Serial.println();
        file = SD.open(path, FILE_WRITE);
        if (file) {
            file.println(head);
            //file.println(message);
        } else {Serial.println("Failed to open file for writing");}
        recFirstRun = false;
       }
       else{
         Serial.println("no valid Time data");
       }
        
    } else {
        Serial.println();
       // Serial.print("RecordMenu--> anyRun: ");
        Serial.println(message);
        Serial.println();
        if (file) {
        file.println(message);
        file.flush();
        }
  }
}

/*void altiFunc::valueDISP(String acutalMenu){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(acutalMenu);
    display.setCursor(0, 8);
    display.println(timestp);
    display.print("Pres: ");
    display.println(pres);
    display.print("Temp:");
    display.print(temp);
    display.setCursor(64, 24);
    display.print("|");
    display.print("Alt:");
    int intAlt = (int) alti;
    display.println(intAlt);
    display.display();
}*/


