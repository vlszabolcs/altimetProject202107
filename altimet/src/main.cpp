#include <Arduino.h>
#include <time.h>

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "SPI.h"
#include <PubSubClient.h>

//My include:
#include "wifiConfig.h"
#include "webDisplay.h"
#include "altifunction.h"

WebServer server(80); 
altiFunc alti;

bool status = true;

void OnConnect() {
  float temperature = alti.temp;
  String timestp = alti.timestp;
  float pressure =  alti.pres;
  float altitude =   alti.alti;
  server.send(200, "text/html", SendHTML(temperature,timestp,pressure,altitude));
};

void NotFound(){
  server.send(404, "text/plain", "Not found");
};

void setup() {

  Serial.begin(115200);

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
    
    if (String(WiFi.SSID()) != "aaa"){
      alti.timeSync();
      DateTime time = rtc.now();
      String filename = (time.timestamp(DateTime::TIMESTAMP_FULL))+"Z";
      filename.replace("-" , "");
      filename.replace(":" , "_");
      alti.path = "/" + filename + ".csv";
    }
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

  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
 
  server.on("/", OnConnect);
  server.onNotFound(NotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {

  alti.logCSV();
  //alti.valueDISP("RecordMen u!");
  server.handleClient();
  delay(1000);
}