#include "main.hpp"

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  wifi_set();
  configTime(9 * 3600, 0, "pool.ntp.org", "time.nist.gov");
}

void loop() {
  delay(3000);
  time_t now = time(nullptr);
  getTextTime(now);
  lux = lightMeter.readLightLevel();
  Serial.println(lux);
  // cursor->execute(INSERT_SQL); 
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