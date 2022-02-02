#include <Arduino.h>
#include <time.h>

//#include <Adafruit_SSD1306.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
#include "SPI.h"

//#include <WiFiMulti.h>
//WiFiMulti wifiMulti;

//My include:
//#include "wifiConfig.h"
#include "altifunction.h"
altiFunc alti;

bool status = true;
int period = 1000;
unsigned long time_now = 0;

void bmp280Setup(){
  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
    }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */  

}

void wifiSetup() {
    /*wifiConnect();
    if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println(WiFi.SSID());
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }*/
}

void firstSetup(){
  //Read data form SD Card
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  File fileRead = SD.open("/lastPresCalc.txt");
  if (fileRead) {
    alti.presCorrig = fileRead.parseFloat();
    Serial.println(alti.presCorrig);
    fileRead.close();
  }
  else {
    Serial.println("Error to open last pressure calc.");
  }


}

void setup() {

  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  //SerialBT.begin("ESPaltimet");

/*
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for (;;); // Don't proceed, loop forever
    }
  display.display();
  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
*/
  
  //wifiSetup();
  bmp280Setup();
  firstSetup();
}

void loop() {

  if(millis() > time_now + period){
      time_now = millis();
      alti.logCSV();
      
  }
   
  alti.gpsData();

   /*if (Serial2.available()){
    Serial.write(Serial2.read());
   }*/
  
  //delay(1000);
}