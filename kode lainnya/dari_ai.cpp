#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TCS34725.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop() {
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC);
  Serial.print("K Lux: "); Serial.println(lux, DEC);

  // Gunakan rumus untuk membedakan warna
  if (r > g && r > b) {
    Serial.println("Warna: Merah");
  } else if (g > r && g > b) {
    Serial.println("Warna: Hijau");
  } else if (b > r && b > g) {
    Serial.println("Warna: Biru");
  } else if (r > b && g > b) {
    Serial.println("Warna: Kuning");
  } else if (r > g && b > g) {
    Serial.println("Warna: Orange");
  } else {
    Serial.println("Warna: Putih");
  }

  delay(1000);
}
