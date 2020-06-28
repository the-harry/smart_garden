#include "DHT.h"

#define LED_GREEN 12
#define LED_RED 13
#define HIGROMETRO A1
#define LDR A2
#define DHTPIN A3
#define DHTTYPE DHT11
#define BUTTON 8
#define RELE_PIN 7

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
    Serial.println("dht_error");
  }
  delay(10);

  air_temp = dht.readTemperature();
  if (isnan(air_temp)) {
    Serial.println("dht_error");
  }
  delay(10);
}

boolean should_irrigate() {
  return (soil_humidity > 800 && luminosidade > 500) || digitalRead(BUTTON) == HIGH;
}

boolean too_much_sun() {
  return soil_humidity > 800 && luminosidade < 500;
}

void send_metrics() {
  Serial.println("{");
  Serial.print("Umidade do solo: ");
  Serial.println(soil_humidity);

  Serial.print("Umidade do ar: ");
  Serial.print(air_humidity);
  Serial.println("%");

  Serial.print("Temperatura: ");
  Serial.println(air_temp);

  Serial.print("Luminosidade: ");
  Serial.println(luminosidade);
  Serial.println("=====================================\n\n");
}

void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(RELE_PIN, OUTPUT);
  Serial.begin(9600);
  dht.begin();
  digitalWrite(RELE_PIN, LOW);
}

void loop() {
  update_metrics();

  if (should_irrigate() == true) {
    Serial.println(" Irrigando horta");
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);

    digitalWrite(RELE_PIN, HIGH);
  } else if (too_much_sun() == true) {
    Serial.println("Solo seco, porem muito sol, abortando irrigacao.");
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);

    digitalWrite(RELE_PIN, LOW);
  } else {
    digitalWrite(RELE_PIN, LOW);
  }

  send_metrics();
  delay(2000);
}
