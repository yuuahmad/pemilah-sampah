#include <ESP32_Servo.h>
#include "Wire.h"
#include "Adafruit_TCS34725.h"
#include <LiquidCrystal_I2C.h>
#define BLYNK_TEMPLATE_ID "TMPL-AFJqLwq"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "qbthkIEuyzWbf2DsZPbkNVJC8_RM7bJb"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Tselhome-03F0";
char pass[] = "71314517";

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myser0;
Servo myser1;
Servo myser2;
Servo myser3;
Servo myser4;
Servo myser5;
Servo myser6;

int ledred     = 25;
int ledblue    = 26;
int ledgreen   = 27;
int ledputih   = 14;
int ledorange  = 12;
String warna_plastik = "";
String jenis_plastik = "";
int jumlah_sampah = 0;
 
// Color Sensor-Objekt initialisieren
// Parameter siehe: https://learn.adafruit.com/adafruit-color-sensors/program-it
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
BlynkTimer timer;
 
void myTimerEvent()
{
  Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V4, warna_plastik);
  Blynk.virtualWrite(V5, jumlah_sampah);
  Blynk.virtualWrite(V6, jenis_plastik);



}

// setup() wird einmal beim Start des Arduino ausgeführt
void setup() {
  //myser0.attach();// servo sensor
  myser1.attach(16,1000,2000);// merah
  myser2.attach(18,1000,2000);// hijau
  myser3.attach(5,1000,2000); // biru
  myser4.attach(17,1000,2000);//orange
  myser5.attach(19,1000,2000);//putih
  
 Serial.begin(115200);
 Serial.println("Pemilah Sampah");
lcd.begin();
  lcd.backlight();
  if (tcs.begin()) {
    
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); 
  }
  pinMode(ledred, OUTPUT);
  pinMode(ledblue, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(ledputih, OUTPUT);
  pinMode(ledorange, OUTPUT);
 
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
    Blynk.run();
  timer.run();
 // Der Sensor liefert Werte für R, G, B und einen Clear-Wert zurück
 uint16_t clearcol, red, green, blue;
 float average, r, g, b;
 delay(100); // Farbmessung dauert c. 50ms 
 tcs.getRawData(&red, &green, &blue, &clearcol);
 
 // Mein Versuch einer Farbbestimmung für 
 // die 5 M&M-Farben Rot, Grün, Blau, Orange und Gelb
 
 // Durchschnitt von RGB ermitteln
 average = (red+green+blue)/3;
 // Farbwerte durch Durchschnitt, 
 // alle Werte bewegen sich jetzt rund um 1 
 r = red/average;
 g = green/average;
 b = blue/average;
 
 // Clear-Wert und r,g,b seriell ausgeben zur Kontrolle
 // r,g und b sollten sich ca. zwischen 0,5 und 1,5 
 // bewegen. Sieht der Sensor rot, dann sollte r deutlich über 1.0
 // liegen, g und b zwischen 0.5 und 1.0 usw.
 Serial.print("\tClear:"); Serial.print(clearcol);
 Serial.print("\tRed:"); Serial.print(r);
 Serial.print("\tGreen:"); Serial.print(g);
 Serial.print("\tBlue:"); Serial.print(b);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Pendeteksi");
  lcd.setCursor(3,1);
  lcd.print("Warna");
  lcd.setCursor(10,1);
 // Versuch der Farbfeststellung anhand der r,g,b-Werte.
 // Am besten mit Rot, Grün, Blau anfangen die die Schwellenwerte
 // mit der seriellen Ausgabe entsprechend anpassen
 if ((r > 1.99) && (g < 0.65) && (b > 0.5)) {
 Serial.print("\tMerah");
 digitalWrite(ledred,HIGH);
 lcd.clear();
 lcd.setCursor(2,0);
 lcd.print("MERAH");
 lcd.setCursor(3,1);
 lcd.print("HDPE");
 delay(1000);
 myser1.write(0);
 delay(1000);
 myser1.write(90);
 digitalWrite(ledred,LOW);
 warna_plastik = "merah";
jenis_plastik = "HDPE";

 jumlah_sampah++;


 }
 else if ((r < 1.2) && (g > 1) && (b < 0.8)) {
 Serial.print("\tHijau");
 digitalWrite(ledgreen,HIGH);
 lcd.clear();
 lcd.setCursor(2,0);
 lcd.print("HIJAU");
 lcd.setCursor(3,1);
 lcd.print("HDPE");
 myser2.write(180);
 delay(3000);
 myser1.write(0);
 delay(2000);
 myser2.write(0);
 myser1.write(90);
 digitalWrite(ledgreen,LOW);
 warna_plastik = "hijau";
jenis_plastik = "HDPE";

 jumlah_sampah++;

 }
 else if ((r < 1.1) && (g < 1.2) && (b > 0.88)) {
 Serial.print("\tBiru");
 lcd.clear();
 digitalWrite(ledblue,HIGH);
 lcd.setCursor(2,0);
 lcd.print("BIRU");
 lcd.setCursor(3,1);
 lcd.print("LDPE");
 myser3.write(180);
 delay(2000);
 myser1.write(0);
 delay(2000);
 myser3.write(70);
 myser1.write(90); 
 digitalWrite(ledblue,LOW);
 warna_plastik = "biru";
jenis_plastik = "LDPE";

 jumlah_sampah++;

 }
 
 else if ((r > 1.38) && (g <0.95) && (b > 0.68)) {
 Serial.print("\tPutih");
 lcd.clear();
 digitalWrite(ledputih,HIGH);
 lcd.setCursor(2,0);
 lcd.print("PUTIH");
 lcd.setCursor(3,1);
 lcd.print("PS");
 myser5.write(180);
 delay(2000);
 myser1.write(0);
 delay(2000);
 myser5.write(70);
 myser1.write(90); 
 digitalWrite(ledputih,LOW);
 warna_plastik = "putih";
jenis_plastik = "PS";

 jumlah_sampah++;

 
 }
 else if ((r > 1.94) && (g > 0.5) && (b > 0.3)) {
 Serial.print("\tOrange");
 digitalWrite(ledorange,HIGH);
 lcd.clear();
 lcd.setCursor(2,0);
 lcd.print("ORANGE");
 lcd.setCursor(3,1);
 lcd.print("PP");
 myser4.write(180);
 delay(3000);
 myser1.write(0);
 delay(4000);
 myser4.write(70);
 myser1.write(90);
 digitalWrite(ledorange,LOW);
 warna_plastik = "oren";
jenis_plastik = "PP";

 jumlah_sampah++;

 
 } 
 else if ((r <1.50) && (g >0.9 ) && (b <0.79)) {
 Serial.print("\tOTHER");
 lcd.clear();
 lcd.setCursor(2,0);
 lcd.print("OTHER");
 delay(2000);
 myser1.write(190);
 delay(2000);
 myser1.write(90);
 warna_plastik = "lainnya";
jenis_plastik = "lainnya";

 jumlah_sampah++;

 } 
 // Wenn keine Regel greift, dann ehrlich sein
 else {
 Serial.print("\tTidak Terdeteksi");
 }
 // Zeilenwechsel ausgeben
 Serial.println("");
 
 // Wartezeit anpassen für serielles Debugging
 //delay(100);
 
}
