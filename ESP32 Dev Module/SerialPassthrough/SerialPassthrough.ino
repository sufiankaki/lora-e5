void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
    while (Serial.available() > 0) {
        Serial2.write(Serial.read());
    }
    while (Serial2.available() > 0) {
        Serial.write(Serial2.read());
    }
}
