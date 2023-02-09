#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <ESP32Servo.h>
#define BLYNK_TEMPLATE_ID "TMPL-AFJqLwq"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "qbthkIEuyzWbf2DsZPbkNVJC8_RM7bJb"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "tempat-sampah";
char pass[] = "11111111";

BlynkTimer timer;

// create four servo objects
Servo servo_sensor;
int minUs = 1000;
int maxUs = 2000;
int servo_sensor_pin = 16; // pada rx2
int nilai_sudut_servo = 0;

int pos = 0; // position in degrees
ESP32PWM pwm;

BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int nilai_sudut_servo_blynk = param.asInt();
  nilai_sudut_servo = nilai_sudut_servo_blynk;
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  Serial.begin(115200);
  servo_sensor.setPeriodHertz(50); // Standard 50hz servo
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
  servo_sensor.attach(servo_sensor_pin, minUs, maxUs);

  servo_sensor.write(nilai_sudut_servo);
  Serial.print(nilai_sudut_servo);
  Serial.println("derajat");


  servo_sensor.detach();
  pwm.detachPin(27);
}