#define RELE_PIN D0
#define LED_RED D1
#define LED_GREEN D2

#define DRY_LIMIT 800
#define WET_LIMIT 1000
#define LIGHT_LIMIT 500

int soil_humidity = 0;
int luminosidade = 0;
boolean manual_irrigation = false;

boolean should_irrigate() {
  return (dry_soil() && !too_much_sun()) || manual_irrigation;
}

boolean dry_soil() {
  return soil_humidity < DRY_LIMIT;
}

boolean wet_soil() {
  return soil_humidity > WET_LIMIT;
}

boolean too_much_sun() {
  return luminosidade > LIGHT_LIMIT;
}

void setup() {
  pinMode(RELE_PIN, OUTPUT);
  Serial.begin(9600);
  digitalWrite(RELE_PIN, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
}

void loop() {
  if (should_irrigate()) {
    Serial.println(" Irrigando horta");

    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(RELE_PIN, HIGH);
  } else if (dry_soil() && too_much_sun()) {
    Serial.println("Solo seco, porem muito sol, abortando irrigacao.");

    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(RELE_PIN, LOW);
  } else if (wet_soil()) {
    Serial.println("Solo muito umido.");

    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(RELE_PIN, LOW);
  } else {
    Serial.println("Tudo ok.");

    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(RELE_PIN, LOW);
  }
}
