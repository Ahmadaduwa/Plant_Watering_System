#include <WiFi.h>
#include "ThingSpeak.h"


// พินเซ็นเซอร์และอุปกรณ์
#define ANALOG_PIN 32       
#define LED_PIN_MOIST 16
#define LED_PIN_DRY 17
#define BUZZER_PIN 5
#define MOTOR_IN1 12
#define MOTOR_IN2 14

// WiFi และ ThingSpeak
const char *SSID = "TEST";                   
const char *PASS = "000011191";              
const unsigned long CHANNEL_ID = 2771163;    
const char *WRITE_API_KEY = "HQC1NLEMHITPKWRO";

WiFiClient  client;

void wifi_reconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(SSID, PASS);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
}

void thingspeak_multi_write() {
  int x = ThingSpeak.writeFields(CHANNEL_ID, WRITE_API_KEY);
  if (x == 200) {
    Serial.println("Multichannel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN_MOIST, OUTPUT);  
  pinMode(LED_PIN_DRY, OUTPUT);   

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  Serial.println("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

int event_before = 0;
int event = 0;

void loop() {
  wifi_reconnect();

  float val = analogRead(ANALOG_PIN);
  float moisture = 100 - (val/4095*100);

  Serial.print("val = ");
  Serial.println(val);

  if (val <= 1600) {
    digitalWrite(LED_PIN_DRY, LOW);
    digitalWrite(LED_PIN_MOIST, HIGH);
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    event = 1;
    Serial.println("Sensor in humid soil");

  } else if (val <= 3000 && val > 1600) {
    digitalWrite(LED_PIN_MOIST, LOW);
    digitalWrite(LED_PIN_DRY, HIGH);
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    event = 0;
    Serial.println("Sensor in dry soil");

  } else {
    digitalWrite(LED_PIN_MOIST, LOW);
    digitalWrite(LED_PIN_DRY, LOW);
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    event = -1;
    Serial.println("Sensor in the Air");
  }

  if (event != event_before){
     tone(BUZZER_PIN, 500, 500);
  }

  event_before = event;

  ThingSpeak.setField(1, moisture);
  ThingSpeak.setField(2, event);
  thingspeak_multi_write();
  delay(100);
}