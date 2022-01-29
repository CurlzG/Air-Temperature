//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FirebaseArduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include<ArduinoJson.h> 
#define ONE_WIRE_BUS 5
// Set these to run example.
#define FIREBASE_HOST " LINK TO REAL TIME DATABASE "
#define FIREBASE_AUTH " AUTH FROM FIREBASE "
#define WIFI_SSID "WIFI NAME "
#define WIFI_PASSWORD "WIFI PASS WORD"
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
const int capacity = JSON_OBJECT_SIZE(3);
StaticJsonBuffer<capacity> jb;
JsonObject& obj = jb.createObject();
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 13 * 60 * 60);
 float Celcius=0;
void setup() {
  Serial.begin(115200);
 
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  sensors.begin();
  timeClient.begin();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;
String text = "Temperature";

void loop() {
  timeClient.update();
  String blank;
  blank += timeClient.getHours();
  blank += ":";
  blank += timeClient.getMinutes();
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  obj["Temperature Recording"] = Celcius;
obj["Time"] = blank;
String text1 = text + n;

  Firebase.set(text1,obj);
  n++;
  delay(15*60*1000);
}
