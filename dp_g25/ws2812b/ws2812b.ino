#include <Wire.h>
#include<ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <NewPing.h>

#include <Adafruit_NeoPixel.h>
  
#define LED_PIN 0

#define LED_COUNT 300

#define API_KEY "AIzaSyDzj7ipac-QzAIccMaJjm6ctaxWJ8jwgno"
#define DATABASE_URL "https://dp-project-e9482-default-rtdb.firebaseio.com/"
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
bool signupOK=false;

int led = 0;
int r = 0;
int g = 0;
int b = 0;
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
    strip.begin();
  strip.show();
}

void loop() {
  if(systemState==1){
    if(Firebase.RTDB.getInt(&fbdo,"/systemState")){
      systemState =  fbdo.intData();
//      if(systemState==0) continue;
    }
    if(Firebase.RTDB.getInt(&fbdo,"/led")){
      led =  fbdo.intData();
    }
    Serial.println(led);
    if(led==1 && systemState){
      r = 225;
      g = 255;
      b = 0;
      if(Firebase.RTDB.getInt(&fbdo,"/colorR")){
      r =  fbdo.intData();
    }
    if(Firebase.RTDB.getInt(&fbdo,"/colorG")){
      g =  fbdo.intData();
    }
    if(Firebase.RTDB.getInt(&fbdo,"/colorB")){
      b =  fbdo.intData();
    }
      for(int i = 0;i<298;i+=3){
        strip.setPixelColor(i,r,g,b);
        strip.setPixelColor(i+1,r,g,b);
        strip.setPixelColor(i+2,r,g,b);
        strip.show();
        delay(5);
      }
    }
    else{
      for(int i = 0;i<300;i++){
      strip.setPixelColor(i, 0, 0, 0);
      strip.show();
      delay(5);
    }
    }
  }
  else{
    if(Firebase.RTDB.getInt(&fbdo,"/systemState")){
      systemState =  fbdo.intData();
     }
     led = 0;
      Firebase.RTDB.setInt(&fbdo,"/led",led);
      r = 0;
      g = 0;
      b = 0;
      for(int i = 0;i<300;i++){
      strip.setPixelColor(i, r, g, b);
      strip.show();
      delay(5);
    }
  }

}
