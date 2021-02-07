#include <Arduino.h>

#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);


void initWifi() {
  //Serial.setDebugOutput(true);
  //system_phy_set_powerup_option(3);
  #ifdef CONFIGURE_WIFI
    WiFi.persistent(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
  #endif
}
void scanWire() {
  byte Return;

  Serial.println("Scanning I2C bus...");
  for(byte I2CAddress = 1; I2CAddress <= 127; I2CAddress++)
  {
    Serial.print("0x");
    if (I2CAddress<16)
      Serial.print("0");
    Serial.print(I2CAddress, HEX);
    Serial.print(" (");
    if (I2CAddress<10)
      Serial.print("  ");
    else if (I2CAddress<100)
      Serial.print(" ");
    Serial.print(I2CAddress);
    Serial.print("): ");

    Wire.beginTransmission(I2CAddress);
    Return = Wire.endTransmission();

    if (Return == 0)
      Serial.print("OK!");
    else
      Serial.print("   ");

    if (I2CAddress % 5)
      Serial.print("    ");
    else
      Serial.println();
  }
}
// ===============================================

void setup() {
  Serial.begin(76800);
  Serial.println("\nBooting... ");

  Wire.begin();

  scanWire();

  Wire.setClock(400000);
  pwm.begin();
  pwm.setPWMFreq(1600);
  pwm.setOutputMode(true);

  for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) {
    pwm.setPWM(pwmnum, 0, 0);
  }
}

void strobeLight(int pin, int d) {
  for (int i=0; i<10; i++) {
    pwm.setPWM(pin, 0, 4095);
    delay(d);
    pwm.setPWM(pin, 0, 1);
    delay(d);
  }
}

void loop() {
  yield();

//    pwm.setPWM(5, 0, 4080);
//return;

  strobeLight(4, 20); //R
  strobeLight(2, 20); //G
  strobeLight(3, 20); //B
  strobeLight(0, 20); //W2
  strobeLight(5, 20); //W1
  strobeLight(1, 20); //X
return;
  Serial.print(".");
  // Drive each PWM in a 'wave'
  for (uint16_t i=10; i<4096; i++) {
    pwm.setPWM(0, 0, i );
    pwm.setPWM(5, 0, 4096-i );
    yield();
  }
  for (uint16_t i=10; i<4096; i++) {
    pwm.setPWM(0, 0, 4096-i );
    pwm.setPWM(5, 0, i );
    yield();
  }
}
