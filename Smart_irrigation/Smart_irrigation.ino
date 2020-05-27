#include "UbidotsESPMQTT.h"
#include <LiquidCrystal.h>

#define TOKEN "A1E-IpeOLxaZrV1NT0YcxSalUAll4fLNR1" // Your Ubidots TOKEN
#define WIFINAME "Jadooi sapola" //Your SSID
#define WIFIPASS "Ranjha#1" // Your Wifi Pass

//for rain sensor
const int capteur_D = 5;

//motor
const int motor = 16;

//soil moisture pin
int sensorPin = A0;
int sensorValue;
int limit = 300;

const int RS = D2, EN = D3, d4 = D5, d5 = D6, d6 = D7, d7 = D8;   
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);

Ubidots client(TOKEN);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void setup()
{
  Serial.begin(115200);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("System");
  
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  
  pinMode(capteur_D, INPUT);
  pinMode(motor, OUTPUT);

  delay(2000);
  lcd.clear();
}

void loop()
{
  if (!client.connected()) {
    client.reconnect();
  }

  //Reading rain sensor
  int rain_check = digitalRead(capteur_D);
  if (rain_check == LOW)
  {
    Serial.print("Digital value : dry ");
    delay(10);
  }
  else
  {
    Serial.print("Digital value : wet ");
    delay(10);
  }
  // reading soil moisture sensor
  sensorValue = analogRead(sensorPin);
  Serial.print("Soil Moisture Value : ");
  Serial.println(sensorValue);

  if (sensorValue < limit) {
    Serial.print("Motor ON");
    digitalWrite(motor, HIGH);  //light the led if soil is wet, else otherwise
  }
  else {
    Serial.print("Motor OFF");
    digitalWrite(motor, LOW);
  }

  client.add("Rain_sensor", rain_check); //Insert your variable Labels and the value to be sent
  client.add("Soil_sensor", sensorValue);
  client.ubidotsPublish("Field-1");
  client.loop();

  delay(1000);
}
