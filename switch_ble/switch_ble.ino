//https://stackoverflow.com/questions/68317473/stop-ble-when-using-wifi
#include "BLEDevice.h"
//#include "BLEScan.h"
#include<string>
#include<iostream>
#include<set>
#include<iterator>
#include <EEPROM.h>

#define RXD2 16
#define TXD2 17
char a[4];
//char b[3];
char addr[17];

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

std::string out;
std::string rssi;
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */

   
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    
    std::string str = advertisedDevice.toString().c_str();
    std::cout << str << "\n";
    std::string M_data;
    std::string address = "e2:15:00:01:b8:6f";
     std::string address1 = "e2:15:00:01:b8:71";
    
   
    if(str.find(address) != std::string::npos){
      //int index = str.find("manufacture");
      std::string str1 = str.substr(66, str.size());//66 th place and 67th have the value of the switch state.
      out = str1.substr(1, 2);
      rssi = str1.substr(18,20);
      for(int i=0;i<2;i++){
       if(out[i]=='0'){
        a[i]='0';
       }
       if(out[i]=='1'){
        a[i]='1';
       }
       if(out[i]=='2'){
        a[i]='2';
       }
       if(out[i]=='3'){
        a[i]='3';
       }if(out[i]=='4'){
        a[i]='4';
       }
       if(out[i]=='5'){
        a[i]='5';
       }
       if(out[i]=='6'){
        a[i]='6';
       }
       if(out[i]=='7'){
        a[i]='7';
       }
       if(out[i]=='8'){
        a[i]='8';
       }
       if(out[i]=='9'){
        a[i]='9';
       }
      }
      for(int i=2;i<4;i++){
      if(rssi[i-1]=='0'){
        a[i]='0';
       }
       if(rssi[i-1]=='1'){
        a[i]='1';
       }
       if(rssi[i-1]=='2'){
        a[i]='2';
       }
       if(rssi[i-1]=='3'){
        a[i]='3';
       }if(rssi[i-1]=='4'){
        a[i]='4';
       }
       if(rssi[i-1]=='5'){
        a[i]='5';
       }
       if(rssi[i-1]=='6'){
        a[i]='6';
       }
       if(rssi[i-1]=='7'){
        a[i]='7';
       }
       if(rssi[i-1]=='8'){
        a[i]='8';
       }
       if(rssi[i-1]=='9'){
        a[i]='9';
       }
       
      }
 
      std::cout<<a<<"out"<<"\n";
//      std::cout<<b<<"rssi"<<"\n";
//       Serial2.println("s");
       Serial2.println(a);
       
      
       
      

    



    
    }

  if(str.find(address1) != std::string::npos){
      //int index = str.find("manufacture");
      std::string str1 = str.substr(66, str.size());//66 th place and 67th have the value of the switch state.
      out = str1.substr(1, 2);
      rssi = str1.substr(18,20);
      for(int i=0;i<2;i++){
       if(out[i]=='0'){
        a[i]='0';
       }
       if(out[i]=='1'){
        a[i]='1';
       }
       if(out[i]=='2'){
        a[i]='2';
       }
       if(out[i]=='3'){
        a[i]='3';
       }if(out[i]=='4'){
        a[i]='4';
       }
       if(out[i]=='5'){
        a[i]='5';
       }
       if(out[i]=='6'){
        a[i]='6';
       }
       if(out[i]=='7'){
        a[i]='7';
       }
       if(out[i]=='8'){
        a[i]='8';
       }
       if(out[i]=='9'){
        a[i]='9';
       }
      }
      for(int i=2;i<4;i++){
      if(rssi[i-1]=='0'){
        a[i]='0';
       }
       if(rssi[i-1]=='1'){
        a[i]='1';
       }
       if(rssi[i-1]=='2'){
        a[i]='2';
       }
       if(rssi[i-1]=='3'){
        a[i]='3';
       }if(rssi[i-1]=='4'){
        a[i]='4';
       }
       if(rssi[i-1]=='5'){
        a[i]='5';
       }
       if(rssi[i-1]=='6'){
        a[i]='6';
       }
       if(rssi[i-1]=='7'){
        a[i]='7';
       }
       if(rssi[i-1]=='8'){
        a[i]='8';
       }
       if(rssi[i-1]=='9'){
        a[i]='9';
       }
       
      }
 
      std::cout<<a<<"out"<<"\n";
//    std::cout<<rssi<<"rssi"<<"\n";
    Serial.println(a);
//       Serial2.println("s");
       Serial2.println(a);
       
      
       
      

    



    
    }

    

    
  }
  

 
   // onResult
}; // MyAdvertisedDeviceCallbacks






void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), false);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(1, false);

}
