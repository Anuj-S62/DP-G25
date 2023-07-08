#include <Wire.h>
#include<ESP8266WiFi.h>
//#include <NewPing.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
//#include <Adafruit_NeoPixel.h>

//#define LED_PIN 0
//#define LED_COUNT 300
#define API_KEY "AIzaSyDzj7ipac-QzAIccMaJjm6ctaxWJ8jwgno"
#define DATABASE_URL "https://dp-project-e9482-default-rtdb.firebaseio.com/"
//Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#define trigPin D5
#define echoPin D6
FirebaseData fbdo;

 long duration;  
 int distance; 

FirebaseAuth auth;
FirebaseConfig config;
bool signupOK=false;
int growLight = 0;
const int ldrPin = 5;
int pipewaterlevel;
int systemState = 1;


WiFiServer server(301); 
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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(D2,HIGH);
}

void loop(){
   digitalWrite(trigPin, LOW);  
 delayMicroseconds(2);  
 // Sets the trigPin on HIGH state for 10 micro seconds  
 digitalWrite(trigPin, HIGH);  
 delayMicroseconds(10);  
 digitalWrite(trigPin, LOW);  
 // Reads the echoPin, returns the sound wave travel time in microseconds  
 duration = pulseIn(echoPin, HIGH);  
 // Calculating the distance  
 distance= duration*0.034/2;  
 // Prints the distance on the Serial Monitor  
 Serial.print("Distance: ");  
 Serial.println(distance); 
 delay(500); 
 Firebase.RTDB.setInt(&fbdo,"/cntWaterLevel",distance);
  if(systemState==1){
  int ldrstatus = analogRead(ldrPin);
  pipewaterlevel = analogRead(A0);
  Serial.println(pipewaterlevel);
  
  Serial.println(ldrstatus);
//  Firebase.RTDB.setInt(&fbdo,"/pipeWaterLevel",pipewaterlevel);
//  Firebase.RTDB.setInt(&fbdo,"/cntWaterLevel",distance);

  
  if(ldrstatus==1023 && growLight==0){
    growLight = 1;
    Firebase.RTDB.setInt(&fbdo,"/growLight",growLight);
    digitalWrite(D2,LOW);
    while(ldrstatus==1023){
      ldrstatus = analogRead(ldrPin);
      pipewaterlevel = analogRead(A0);
      delay(10);
      digitalWrite(trigPin, LOW);  
 delayMicroseconds(2);  
 // Sets the trigPin on HIGH state for 10 micro seconds  
 digitalWrite(trigPin, HIGH);  
 delayMicroseconds(10);  
 digitalWrite(trigPin, LOW);  
 // Reads the echoPin, returns the sound wave travel time in microseconds  
 duration = pulseIn(echoPin, HIGH); 
  distance= duration*0.034/2;  
      Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);
  Firebase.RTDB.setInt(&fbdo,"/cntWaterLevel",distance);
      Firebase.RTDB.setInt(&fbdo,"/pipeWaterLevel",pipewaterlevel);
      if(Firebase.RTDB.getInt(&fbdo,"/growLight")){
        growLight = fbdo.intData();
     }
     if(Firebase.RTDB.getInt(&fbdo,"/systemState")){
        systemState = fbdo.intData();
        if(systemState==0){
          break;
        }
     }
      if(ldrstatus==0){
        growLight = 0;
        Firebase.RTDB.setInt(&fbdo,"/growLight",growLight);
      }
      if(growLight==0){
        digitalWrite(D2,HIGH);
      }
      else{
        digitalWrite(D2,LOW);
      }
      delay(1000);
    }    
  }
  if(Firebase.ready() && signupOK){
     if(Firebase.RTDB.getInt(&fbdo,"/growLight")){
        growLight = fbdo.intData();
        if(growLight==1)digitalWrite(D2,LOW);
        else digitalWrite(D2,HIGH);
     }
  }

  delay(1000); 
  }
  else{
    digitalWrite(D2,HIGH);
    growLight = 0;
       Firebase.RTDB.setInt(&fbdo,"/growLight",growLight);
    Serial.println("System is OFF");
    if(Firebase.RTDB.getInt(&fbdo,"/systemState")){
        systemState = fbdo.intData();
     }
     delay(1000);
  }
}
