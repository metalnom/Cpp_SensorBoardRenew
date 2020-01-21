#include <Arduino.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

byte mac_addr[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress server_addr(192,168,10,156);
char user[] = "node";
char password[] = "node";
char INSERT_SQL[] = "insert into test.hello (message) values ('hello')";

char ssid[] = "iptime";
char pwd[] = "";

WiFiClient client;
MySQL_Connection conn((Client *)&client);
MySQL_Cursor* cursor;

void wifi_set(void);
void getTextTime(time_t now);

void setup() {
  Serial.begin(9600);
  wifi_set();
  configTime(9 * 3600, 0, "pool.ntp.org", "time.nist.gov");
}

void loop() {
  delay(3000);
  time_t now = time(nullptr);
  getTextTime(now);
  cursor->execute(INSERT_SQL); 
}

void wifi_set(void) {
  Serial.printf("\nConnecting to %s", ssid);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to network");
  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to SQL...  ");
  if (conn.connect(server_addr, 3306, user, password))
    Serial.println("OK.");
  else
    Serial.println("FAILED.");
  
  cursor = new MySQL_Cursor(&conn);  
}

void getTextTime(time_t now) {
  struct tm *timeinfo;
  timeinfo = localtime(&now);
  String text = String(timeinfo->tm_year+1900) + " " + String(timeinfo->tm_mon+1) + " " \
                + String(timeinfo->tm_mday) + " " + String(timeinfo->tm_hour) + ":" \
                + String(timeinfo->tm_min) + ":" + String(timeinfo->tm_sec);
  Serial.println(text);
}