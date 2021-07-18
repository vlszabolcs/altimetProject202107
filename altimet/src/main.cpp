#include <Arduino.h>

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "SPI.h"
#include <PubSubClient.h>

//Saját:
#include "wifiConfig.h"

#include "altifunction.h"

altiFunc alti;
bool status = true;
String menu;



void setup() {

  Serial.begin(115200);
  Serial.println("Serial started");

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for (;;); // Don't proceed, loop forever
    }
  display.display();
  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);

  wifiConnect();
  
  if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      Serial.flush();
      abort();
    }
  
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println(WiFi.SSID());
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    alti.timeSync();
  }

  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
    }

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

void loop() {
  
  alti.logCSV();
  
  if (status == true){
  menu="RecordMenu!";
  status=false;
  }
    else if (status==false){
    menu="";
    status=true;
  }
  alti.valueDISP(menu);
  delay(1000);
}