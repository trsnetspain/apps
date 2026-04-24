#include <Adafruit_Si4713.h>
#include <Wire.h>

#define RESET_PIN 12
#define SI4713_PROP_TX_RDS_PI 0x2C01

Adafruit_Si4713 radio = Adafruit_Si4713(RESET_PIN);

void setup() {
  Serial.begin(9600);
  if (!radio.begin(0x63)) while (1);
  
  radio.tuneFM(8800);
  radio.setTXpower(115);
  radio.setProperty(SI4713_PROP_TX_COMPONENT_ENABLE, 0x0005); // Mono + RDS
  radio.setProperty(SI4713_PROP_TX_RDS_PI, 0x1234);           // PI por defecto
  radio.setProperty(SI4713_PROP_TX_AUDIO_DEVIATION, 6625); 
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    delay(30);
    String val = Serial.readStringUntil('\n');
    val.trim();

    switch(cmd) {
      case 'P': radio.setTXpower(val.toInt()); break;               
      case 'S': { char p[9]; val.toCharArray(p, 9); radio.setRDSstation(p); break; } 
      case 'T': radio.setRDSbuffer(val.c_str()); break;
      case 'I': radio.setProperty(SI4713_PROP_TX_RDS_PI, strtol(val.c_str(), NULL, 16)); break;
    }
  }
}