#include <Arduino.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_BMP085.h>

IPAddress server_addr(192,168,10,156);
char user[] = "node";
char password[] = "node";
char INSERT_SQL[] = "insert into test.hello (message) values ('hello')";

char ssid[] = "iptime";
char pwd[] = "";

WiFiClient client;
MySQL_Connection conn((Client *)&client);
MySQL_Cursor* cursor;

BH1750 LightSensor(0x23);
float light = 0.0;

Adafruit_BMP085 PressureSensor;
float pressure = 0.0;

void wifi_set(void);
void getTextTime(time_t now);