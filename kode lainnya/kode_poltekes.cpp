#include "Servo.h"
#include "Wire.h"
#include "Adafruit_TCS34725.h"
#include <LiquidCrystal_I2C.h>
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

// Color Sensor-Objekt initialisieren
// Parameter siehe: https://learn.adafruit.com/adafruit-color-sensors/program-it
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
 
// setup() wird einmal beim Start des Arduino ausgeführt
void setup() {
  //myser0.attach();// servo sensor
  myser1.attach(16);// merah
  myser2.attach(18);// hijau
  myser3.attach(5); // biru
  myser4.attach(17);//orange
  myser5.attach(19);//putih
  
 Serial.begin(9600);
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
 
 
}
 

void loop() {
 
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
