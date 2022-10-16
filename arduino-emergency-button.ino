#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "config.h"

int buzzer = 5; // D1(gpio5)
int button = 4; // D2(gpio4)
int buttonState=0;
int buttonStateOld=0; 

WiFiClientSecure client;

void initWIFI() {
    Serial.println();
    Serial.print("Url: ");
    Serial.println(actionURL);
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());

    Serial.print("Connecting to ");
    Serial.println(wifiSID);

    Serial.flush();

    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSID, wifiPassword);

    int i=0;
    while (WiFi.status() != WL_CONNECTED && i<40) {
      Serial.print(".");
      delay(1000);
      i++;
    }
    
    if (WiFi.status() == WL_CONNECTED){
      Serial.println("WIFI connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }else{
      Serial.println("WIFI is NOT connected");
    }
}

void triggerAction(){
 if(WiFi.status()== WL_CONNECTED){
      Serial.println("lets send something");
      HTTPClient http;
      
      http.begin(client, actionURL);

      http.addHeader("Accept", "application/vnd.github.everest-preview+json");
      http.addHeader("Authorization", "token " + ghToken);

      // Data to send with HTTP POST
      String httpRequestData = "{\"event_type\": \"do-something\"}";

      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      http.end();
 }else{
  Serial.println("Trigger failed, WIFI is NOT connected");
 }
}

void beep(int count){
  for (int i = 0; i < count; i++) {
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
  }
}

void initializeSerial() {
  Serial.begin(115200);
  Serial.println();
  Serial.setTimeout(2000);
  Serial.flush();
}

void setup() {
 pinMode(buzzer, OUTPUT);
 pinMode(button, INPUT);
 digitalWrite(buzzer, HIGH);
 initializeSerial();
 Serial.print("Ready!");
 initWIFI();
 client.setTimeout(5000);
 client.setInsecure();
}

void loop() {
 buttonState=digitalRead(button);
 if (buttonStateOld != buttonState) {
   buttonStateOld = buttonState;
   if (buttonState == 1) {
    Serial.println("pressed");
    beep(1);
    triggerAction();
    beep(2);
    delay(200);
   }
   if (buttonState==0)
   {
   Serial.println("released");
   delay(200);
   }
 }
}
