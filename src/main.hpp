#include <Arduino.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

IPAddress server_addr(192,168,10,156);
char user[] = "node";
char password[] = "node";
char INSERT_syn[] = "insert into test.sensor (light, pre, tem, hum) values";
char INSERT_val[100];
char INSERT_SQL[150];

char ssid[] = "iptime";
char pwd[] = "";

WiFiClient client;
MySQL_Connection conn((Client *)&client);
MySQL_Cursor* cursor;

BH1750 LightSensor(0x23);
float light = 0.0;

Adafruit_BMP085 PreTemSensor;
float pressure = 0.0;
float temperature = 0.0;

DHT HumiditySensor(D5, DHT11);
float humidity = 0.0;

void wifi_set(void);
void getTextTime(time_t now);
void console_print(time_t t);
void insert_mod(void);