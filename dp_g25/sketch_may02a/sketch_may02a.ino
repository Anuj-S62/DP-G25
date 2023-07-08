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

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
bool signupOK=false;

WiFiServer server(301); 
int pumpState = 0;
int growLight = 0;
const int ldrPin = 5;
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
  pinMode(D2,OUTPUT);
  pinMode(ldrPin,INPUT);
  digitalWrite(D2,LOW);
}

void loop(){
    Serial.print("Pump: ");
  Serial.println(pumpState);
  int ldrstatus = analogRead(ldrPin);
  Serial.println(ldrstatus);
  
  if(ldrstatus==1023){
    growLight = 1;
    Firebase.RTDB.setInt(&fbdo,"/growLight",growLight);
  }
  else{
    growLight = 0;
    Firebase.RTDB.setInt(&fbdo,"/growLight",growLight);
  }
  if(Firebase.ready() && signupOK){
     if(Firebase.RTDB.getInt(&fbdo,"/growLight")){
        growLight = fbdo.intData();
        if(growLight==1)digitalWrite(D2,LOW);
        else digitalWrite(D2,HIGH);
     }
  }

 Serial. println("--------------");
  delay(200); 
}
