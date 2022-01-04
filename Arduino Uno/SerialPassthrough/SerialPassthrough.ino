#include<SoftwareSerial.h>

SoftwareSerial e5(7, 6);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  e5.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
      while (Serial.available() > 0) {
        e5.write(Serial.read());
    }
    while (e5.available() > 0) {
        Serial.write(e5.read());
    }
}
