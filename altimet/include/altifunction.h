// Create a Car class with some attributes
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org",7200);

#include "RTClib.h"
RTC_DS3231 rtc;

#include "SD.h"

#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


class altiFunc {
    public:
        String message;
        String timestp;
        String path;
        File file;
        float pres;
        float temp ;
        float alti ;
        float presCorrig;
        void sensData();
        void timeSync ();
        void logCSV();
        //void valueDISP(String acutalMenu);
    private:
       bool recFirstRun=true;
       
};

void altiFunc::sensData () {
  DateTime time = rtc.now();
  timestp = (time.timestamp(DateTime::TIMESTAMP_FULL))+"Z";
 //timestp = timestp+"Z";
  pres = bmp.readPressure() / 100.0F;
  temp = bmp.readTemperature()-2;
  alti = bmp.readAltitude(presCorrig);
  message = timestp + "," + String(pres) + "," + String(temp) + "," + String(alti);

}

void altiFunc::timeSync () {
  timeClient.begin();
  timeClient.update();
  rtc.adjust(DateTime(timeClient.getEpochTime()));
  DateTime time = rtc.now();
  Serial.println(String("Time Synchronized:\t") + time.timestamp()); 
}

void altiFunc:: logCSV(){
    sensData();
    if (recFirstRun == true) {
       ///creat file name 
        String filename = timestp;
        filename.replace("-" , "");
        filename.replace(":" , "_");
        path = "/" + filename + ".csv";
        String head = "Time,Pressure,Temperature,Altitude";
        Serial.print("RecordMenu--> FirstRun: ");
        Serial.println(path);
        Serial.println(message);
        Serial.println();
        file = SD.open(path, FILE_WRITE);
        if (file) {
            file.println(head);
            //file.println(message);
        } else {Serial.println("Failed to open file for writing");}
        recFirstRun = false;
        
    } else {
       
        Serial.print("RecordMenu--> anyRun: ");
        Serial.println(message);
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


