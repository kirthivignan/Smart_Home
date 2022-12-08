//WIFI BOARD
#include <bits/stdc++.h>
#include <WiFi.h>
#include "time.h"
#include <HTTPClient.h>
//for OTA
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>





const char* ssid     = "CBS Beam";
const char* password = "Bsrc12#$";
const char* serverName = "http://192.168.1.170:5000/iot";//server ip
//int out=0,rssi=0;
String out,rssi,outf,outn;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
int flag=0;
String response2,response3;
const int   daylightOffset_sec = 3600;
#define RXD2 16
#define TXD2 17
int LED =25;
int FAN =26;


void setup(){
  Serial.begin(115200);
 Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
   WiFi.mode(WIFI_STA);
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  pinMode(LED,OUTPUT);
  pinMode(FAN,OUTPUT);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}




//2022-08-10 12:22:31
void loop(){
 ArduinoOTA.handle();
 struct tm timeinfo;
 getLocalTime(&timeinfo);
 char k[20];
  strftime(k,20,"%Y-%m-%d %H:%M:%S",&timeinfo);
//  Serial.println(k);
//while (Serial2.available() == 0) { }  
//String start = Serial2.readString();
//start.trim();


while (Serial2.available() != 0)
 {out =Serial2.readString();   
out.trim();}
outf = out.substring(0,2);
rssi=out.substring(2,4);





//
//Serial.print("out : "); 
//Serial.println(outf);
//Serial.println(out);
//Serial.print("rssi : "); 
//Serial.println(rssi);



if (WiFi.status() == WL_CONNECTED&&out!=outn) {
      WiFiClient client;
      HTTPClient http;
        http.begin(client, serverName);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        
        String httpRequestData = "mac=100&time=";
         String timei = "";
    for (int i = 0; i < 19; i++) {
        timei = timei + k[i];
    }



        httpRequestData=httpRequestData+(timei);
        httpRequestData=httpRequestData+("&signal=");
        httpRequestData=httpRequestData+(rssi);
        httpRequestData=httpRequestData+("&device=Cabin&action=");
        httpRequestData=httpRequestData+(outf);
        
        

        
//       Serial.println(timei);
      int httpResponseCode = http.POST(httpRequestData);
      
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String response = http.getString(); 
       Serial.println(response);
      
       response2=response;
      // Free resources
     if(httpResponseCode!=-1){
        outn=out;
     }     
      http.end();
   
    

 }
  Serial.println(response2);
  Serial.println(response2.substring(11,13));
 if(response2.substring(11,13)=="02"||response2.substring(11,13)=="03"){
     
        digitalWrite(LED,HIGH);
        Serial.println("Glow");
        }
if(response2.substring(11,13)=="05"||response2.substring(11,13)=="04")   {  
        digitalWrite(LED,LOW);
        Serial.println("No Glow");
        
    }
 if(response2.substring(11,13)=="08"||response2.substring(11,13)=="09"){
     
        digitalWrite(FAN,HIGH);
        Serial.println("Glow");
        }
if(response2.substring(11,13)=="11"||response2.substring(11,13)=="10")   {  
        digitalWrite(FAN,LOW);
        Serial.println("No Glow");
        
    }


    
}
