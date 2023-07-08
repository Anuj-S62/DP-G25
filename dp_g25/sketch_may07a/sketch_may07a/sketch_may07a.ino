#include <Wire.h>
#include<WiFi.h>
#include "DHT.h"

#include <Firebase_ESP_Client.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#define API_KEY "AIzaSyDzj7ipac-QzAIccMaJjm6ctaxWJ8jwgno"
#define DATABASE_URL "https://dp-project-e9482-default-rtdb.firebaseio.com/"
#define DHTTYPE DHT22 
const int oneWireBus = 0;   
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
//DHTesp dht;

#define DHTPIN 4 
DHT dht(DHTPIN, DHTTYPE);
FirebaseData fbdo;



FirebaseAuth auth;
FirebaseConfig config;
bool signupOK=false;

WiFiServer server(301); 
int pumpState = 0;
float threshold = 28.0;
float currph = 7.7;
float thresholdph = 7.7;
bool phflag = true;
int systemState = 0;
const int potPin=A0;
float ph;
float Value=0;
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
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  pinMode(15,OUTPUT);
  digitalWrite(15,LOW);
  dht.begin();
  pinMode(potPin,INPUT);
  sensors.begin();
}

void loop(){
  if(systemState){
  if(phflag){
//    float humidity = dht.readHumidity();
//  float temperature = dht.readTemperature();
  sensors.requestTemperatures(); 
  float waterTemp = sensors.getTempCByIndex(0);
  Value= analogRead(potPin);
    Serial.print(Value);
    Serial.print(" | ");
    float voltage=Value*(3.3/4095.0);
    ph=(4.46*voltage);
    currph = ph;
    if(currph<thresholdph){
      phflag = false;
    }
   Serial.print("Ph:");
   Serial.println(ph);
//  Serial.print("Surr temperature: ");
//  Serial.println(temperature);
//  Serial.print("Humidity: ");
//  Serial.println(humidity);
  Serial.print("waterTemp: ");
  Serial.println(waterTemp);
    Serial.print("Pump: ");
  Serial.println(pumpState);
  digitalWrite(15,HIGH);
  if(Firebase.ready() && signupOK){
     if(Firebase.RTDB.getInt(&fbdo,"/pumpState")){
        pumpState = fbdo.intData();
        if(pumpState==1)digitalWrite(2,LOW);
        else digitalWrite(2,HIGH);
     }
     if(Firebase.RTDB.getFloat(&fbdo,"/tempThreshold")){
        threshold = fbdo.floatData();
     }
     if(Firebase.RTDB.getInt(&fbdo,"/systemState")){
        systemState = fbdo.intData();
     }
     if(Firebase.RTDB.getFloat(&fbdo,"/pHThreshold")){
        thresholdph = fbdo.floatData();
        if(thresholdph > currph){
          phflag = false;
        }
     }
     Firebase.RTDB.setString(&fbdo,"/ph",currph);
//     Firebase.RTDB.setString(&fbdo,"/humidity",humidity);
//     Firebase.RTDB.setString(&fbdo,"/surrtemp",temperature);
     Firebase.RTDB.setString(&fbdo,"/waterTemp",waterTemp);
  }
  if(waterTemp>=threshold && phflag){
    Serial.println("--waterTemp is high--");
    pumpState = 1;
    digitalWrite(2,LOW);
    Firebase.RTDB.setInt(&fbdo,"/pumpState",pumpState);
    while(waterTemp >= threshold && phflag){
      Serial.println("Hello world");
      if(Firebase.RTDB.getFloat(&fbdo,"/tempThreshold")){
        threshold = fbdo.floatData();
     }
     if(Firebase.RTDB.getInt(&fbdo,"/pumpState")){
        pumpState = fbdo.intData();
        if(pumpState==1)digitalWrite(2,LOW);
        else digitalWrite(2,HIGH);
     }
//      humidity = dht.readHumidity();
//      temperature = dht.readTemperature();
      sensors.requestTemperatures(); 
      waterTemp = sensors.getTempCByIndex(0);
      Value= analogRead(potPin);
    Serial.print(Value);
    Serial.print(" | ");
    float voltage=Value*(3.3/4095.0);
    ph=(4.6*voltage);
    currph = ph;
    if(currph<thresholdph){
      phflag = false;
      break;
    }
   Serial.print("Ph:");
   Serial.println(ph);
      if(Firebase.ready()){
        Firebase.RTDB.setString(&fbdo,"/ph",currph);
//        Firebase.RTDB.setString(&fbdo,"/humidity",humidity);
//     Firebase.RTDB.setString(&fbdo,"/surrtemp",temperature);
       Firebase.RTDB.setString(&fbdo,"/waterTemp",waterTemp);
      }
      Serial.print("Ph:");
   Serial.println(ph);
      Serial.print("waterTemp : ");
      Serial.println(waterTemp);
//  Serial.print("Surr temperature: ");
//  Serial.println(temperature);
//  Serial.print("Humidity: ");
//  Serial.println(humidity);
    }
    pumpState = 0;
    digitalWrite(2,HIGH);
    Serial.println();
    Firebase.RTDB.setInt(&fbdo,"/pumpState",pumpState);
  }
// Serial. println("--------------");
  }
  else{
    digitalWrite(15,LOW);
    while(currph < thresholdph){
      Value= analogRead(potPin);
    Serial.print(Value);
    Serial.print(" | ");
    float voltage=Value*(3.3/4095.0);
    ph=(4.6*voltage);
    currph = ph;
//    digitalWrite(15,LOW);
      Firebase.RTDB.setString(&fbdo,"/ph",currph);
      if(Firebase.RTDB.getFloat(&fbdo,"/pHThreshold")){
        thresholdph = fbdo.floatData();
     }
     delay(100);
    }
    digitalWrite(15,HIGH);
    phflag = true;
  }
  }
  else{
    Serial.println("System is OFF");
    digitalWrite(2,HIGH);
    digitalWrite(15,HIGH);
    pumpState = 0;
    Firebase.RTDB.setInt(&fbdo,"/pumpState",pumpState);
    
    if(Firebase.RTDB.getInt(&fbdo,"/systemState")){
        systemState = fbdo.intData();
     }
  }
}
