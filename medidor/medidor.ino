#include "DHT.h"
#include <ArduinoJson.h>

#define HIGROMETRO D0
#define LDR D1
#define DHTPIN A0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int soil_humidity = 0;
int luminosidade = 0;
float air_humidity = 0;
float air_temp = 0;

void update_metrics() {
  soil_humidity = analogRead(HIGROMETRO);
  delay(10);

  luminosidade = analogRead(LDR);
  delay(10);

  air_humidity = dht.readHumidity();
  if (isnan(air_humidity)) {
    // air_humidity = 0;
    Serial.println("dht_error");
  }
  delay(10);

  air_temp = dht.readTemperature();
  if (isnan(air_temp)) {
    // air_temp = 0;
    Serial.println("dht_error");
  }
  delay(10);
}

void build_metrics() {
  update_metrics();

  StaticJsonDocument<200> doc;

  doc["soil_humidity"] = soil_humidity;
  doc["air_humidity"] = float(air_humidity);
  doc["air_temp"] = float(air_temp);
  doc["light"] = luminosidade;

  serializeJson(doc, Serial);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  build_metrics();
  delay(1000);
}
