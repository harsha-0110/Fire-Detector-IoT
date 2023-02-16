#include <WiFi.h>
#include <DHT.h>
#include "ThingSpeak.h"

int DHT_PIN = 7;
int buzzer = 13;
DHT dht(DHT_PIN, DHT22);
const char* ssid = "WIFI-SSID";
const char* pass = "WIFI-PASS";
WiFiClient  client;
unsigned long myChannelNumber = CHANNEL-ID;
const char * myWriteAPIKey = API-KEY;
float temperatureC;
float humidity;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(buzzer, OUTPUT);
  Serial.println("Connecting to");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  ThingSpeak.begin(client);
}

void loop() {
  temperatureC = dht.readTemperature();
  Serial.print("Temperature (ºC): ");
  Serial.println(temperatureC);
  humidity = dht.readHumidity();
  Serial.print("Humidity (%): ");
  Serial.println(humidity);
  ThingSpeak.setField(1, temperatureC);
  ThingSpeak.setField(2, humidity);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(temperatureC >= 10) {
    digitalWrite(buzzer, HIGH);
    delay(1000);
  }
  digitalWrite(buzzer, LOW);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(1000); 
}
