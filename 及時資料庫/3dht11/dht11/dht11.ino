// Firebase
// 使用arduino nano 33 iot

/*
 *連線Firebase realtimeDatabase
 *get節點資料
*/

//Example shows how to connect to Firebase RTDB and perform basic operation for set, get, push and update data to database
//Required WiFiNINA Library for Arduino from https://github.com/arduino-libraries/WiFiNINA
//https://github.com/mobizt/Firebase-Arduino-WiFiNINA

#include "Firebase_Arduino_WiFiNINA.h"
#include "DHT.h"

#define FIREBASE_HOST "arduinofirebase-6d104.firebaseio.com"
#define FIREBASE_AUTH "z5lPWwjZLZuNNcUEelbJdiNaIvnR2Zfq49BuQBAa"
#define WIFI_SSID "robert_hsu_home"
#define WIFI_PASSWORD "0926656000"
#define dhtData 8
//Define Firebase data object
FirebaseData firebaseData;
DHT dht(dhtData,DHT11);

void setup()
{
  Serial.begin(9600);
  delay(100);
  Serial.println();

  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //Provide the autntication data
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);

  //DHTinit
  dht.begin();
}

 

void loop()
{
  float h = dht.readHumidity();//讀取濕度
  float t = dht.readTemperature();//讀取攝氏溫度
  float f = dht.readTemperature(true);//讀取華氏溫度
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("無法從DHT傳感器讀取！");    
  }
  Serial.print("濕度: ");
  Serial.print(h);
  Serial.print("%\t");
  Serial.print("攝氏溫度: ");
  Serial.print(t);
  Serial.print("*C\t");
  Serial.print("華氏溫度: ");
  Serial.print(f);
  Serial.print("*F\n");
  
  String humidityPath = "DHT11/humidity";
  String temperaturePath = "DHT11/temperature";

  if(Firebase.setFloat(firebaseData,humidityPath,h)){
    if(firebaseData.dataType() == "float")
      Serial.println(firebaseData.floatData());
  }else{
    Serial.println(firebaseData.errorReason());
  }

  if(Firebase.setFloat(firebaseData,temperaturePath,t)){
    if(firebaseData.dataType() == "float")
      Serial.println(firebaseData.floatData());
  }else{
    Serial.println(firebaseData.errorReason());
  }
 
  delay(2000);
}
