#include <Wire.h>
#include<ESP8266WiFi.h>
#include "DHTesp.h"
#include <Firebase_ESP_Client.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyDzj7ipac-QzAIccMaJjm6ctaxWJ8jwgno"
#define DATABASE_URL "https://dp-project-e9482-default-rtdb.firebaseio.com/"
#define ONE_WIRE_BUS D4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DHTesp dht;
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
bool signupOK=false;

WiFiServer server(301); 
int pumpState = 0;
float threshold = 28.0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.localIP());
  Serial.print("Start connection");
  WiFi.begin("Mi 11X","12345a678");
  while((!(WiFi.status()== WL_CONNECTED))){
      Serial.print("..");
      delay(1000);
    }
     server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  Serial.println("Connected");     
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.print("world");
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase is Ready");
  dht.setup(5, DHTesp::DHT22);
  pinMode(D2,OUTPUT);
  digitalWrite(D2,LOW);
  sensors.begin();
}

void loop(){
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  sensors.requestTemperatures(); 
  float waterTemp = sensors.getTempCByIndex(0);
  Serial.print("Surr temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("waterTemp: ");
  Serial.println(waterTemp);
    Serial.print("Pump: ");
  Serial.println(pumpState);
  if(Firebase.ready() && signupOK){
     if(Firebase.RTDB.getInt(&fbdo,"/pumpState")){
        pumpState = fbdo.intData();
        if(pumpState==1)digitalWrite(D2,LOW);
        else digitalWrite(D2,HIGH);
     }
     if(Firebase.RTDB.getFloat(&fbdo,"/tempThreshold")){
        threshold = fbdo.floatData();
     }
     Firebase.RTDB.setString(&fbdo,"/humidity",humidity);
     Firebase.RTDB.setString(&fbdo,"/surrtemp",temperature);
     Firebase.RTDB.setString(&fbdo,"/waterTemp",waterTemp);
  }

  if(waterTemp>=threshold){
    Serial.println("--waterTemp is high--");
    pumpState = 1;
    digitalWrite(D2,LOW);
    Firebase.RTDB.setInt(&fbdo,"/pumpState",pumpState);
    while(waterTemp >= threshold){
      if(Firebase.RTDB.getFloat(&fbdo,"/tempThreshold")){
        threshold = fbdo.floatData();
     }
     if(Firebase.RTDB.getInt(&fbdo,"/pumpState")){
        pumpState = fbdo.intData();
        if(pumpState==1)digitalWrite(D2,LOW);
        else digitalWrite(D2,HIGH);
     }
      humidity = dht.getHumidity();
      temperature = dht.getTemperature();
      sensors.requestTemperatures(); 
      waterTemp = sensors.getTempCByIndex(0);
      if(Firebase.ready()){
        Firebase.RTDB.setString(&fbdo,"/humidity",humidity);
     Firebase.RTDB.setString(&fbdo,"/surrtemp",temperature);
       Firebase.RTDB.setString(&fbdo,"/waterTemp",waterTemp);
      }
      Serial.print("waterTemp : ");
      Serial.println(waterTemp);
  Serial.print("Surr temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
    }
    pumpState = 0;
    digitalWrite(D2,HIGH);
    Serial.println();
    Firebase.RTDB.setInt(&fbdo,"/pumpState",pumpState);
  }
  if(pumpState==1){
    digitalWrite(D2,LOW);
  }
  else{
    digitalWrite(D2,HIGH);
  }
 Serial. println("--------------");
  delay(200); 
}
