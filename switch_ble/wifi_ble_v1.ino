// BLE
#include "BLEDevice.h"
#include <EEPROM.h>

// WIFI
#include <WiFi.h>
#include "time.h"
#include <HTTPClient.h>

//Cpp libraries
#include <bits/stdc++.h>

// BLE var


static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static BLEAdvertisedDevice *myDevice;
std::string out;
std::string out1;
std::string rssi;
String outf,rssi1;
String mac;


// WIFI var
const char *ssid ="TP-Link_A0A7";                                   //"PATRIoT@301_2.4G";             //"Kirthi Vignan's iPhone";                   // "CBS Beam"
const char *password ="patriot123";                               //"PATRIoT@301_2.4G";//"9948825566";                                               // "Bsrc12#$"
const char *serverName ="http://192.168.0.160:5000/iot";        // server ip


//TIME VAR 
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
struct tm timeinfo;
const int daylightOffset_sec = 3600;




//HTTP RESPONSE
String server_response;
int count = 0,counter=0;




class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {

    std::string str = advertisedDevice.toString().c_str();
    //    std::cout << str << "\n";
    std::string M_data;
    std::string address = "e2:15:00:01:b8:6f";
    std::string address1 = "e2:15:00:01:b8:71";

    if (str.find(address) != std::string::npos)
    {
      // int index = str.find("manufacture");
      std::string str1 = str.substr(66, str.size()); // 66 th place and 67th have the value of the switch state.
      out = str1.substr(1, 2);
      rssi = str1.substr(18, 20);
      mac="e2:15:00:01:b8:6f";
      counter++;
      Serial.println(out.c_str());
      Serial.println(rssi.c_str());
    }

    if (str.find(address1) != std::string::npos)
    {
      // int index = str.find("manufacture");
      std::string str1 = str.substr(66, str.size()); // 66 th place and 67th have the value of the switch state.
      out = str1.substr(1, 2);
      rssi = str1.substr(18, 20);
       mac="e2:15:00:01:b8:71";
       counter++;
      Serial.println(out.c_str());
      Serial.println(rssi.c_str());
      

    }
  }


}; // MyAdvertisedDeviceCallbacks

//Function to run BLE
void BLE()
{   Serial.println(".");
//  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), false);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(1, false);
}





void WIFI()
{
  Serial.print("Connecting to ");
  WiFi.mode(WIFI_STA);
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
 
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  getLocalTime(&timeinfo);
  Serial.println( getLocalTime(&timeinfo));







 char k[20];
 strftime(k,20,"%Y-%m-%d %H:%M:%S",&timeinfo);
 
if (WiFi.status() == WL_CONNECTED) {
      count++;
      WiFiClient client;
      HTTPClient http;
        http.begin(client, serverName);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        
        String httpRequestData = "mac=";
        httpRequestData+=mac;
        httpRequestData+="&&time=";
         String timei = "";
    for (int i = 0; i < 19; i++) {
        timei = timei + k[i];
    }
      
    Serial.println(timei);
        httpRequestData=httpRequestData+(timei);
        httpRequestData=httpRequestData+("&signal=");
        httpRequestData=httpRequestData+rssi.c_str();//rssi
        httpRequestData=httpRequestData+("&device=esp2&action=");//change this for room value
        httpRequestData=httpRequestData+out.c_str();//out
        
        

      int httpResponseCode = http.POST(httpRequestData);
      
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String response = http.getString(); 
       Serial.println(response);
      
       server_response=response;
 
     
      http.end();
   
    

 }
  Serial.println(server_response);
  Serial.println(server_response.substring(11,13));

}

void setup()
{
  Serial.begin(115200);
}


int i=0;
void loop()
{ while(count==counter)
  { 
    BLE();
  }
  BLEDevice::deinit(ESP.getFreeHeap());
  Serial.println("------------------------------------------------");
  Serial.println(" \t:::: Bluetooth OFF :::: ");
  Serial.println("------------------------------------------------");
  delay(100);
  WIFI();
  
  delay(100);
  WiFi.disconnect(true);
   Serial.println("------------------------------------------------");
  Serial.println(" \t:::: WIFI OFF :::: ");
  Serial.println("------------------------------------------------");
  ESP.restart();
}
