#include "main.hpp"

void setup() {
  Serial.begin(9600);
  Wire.begin();
  LightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  PreTemSensor.begin();
  HumiditySensor.begin();
  wifi_set();
  configTime(9 * 3600, 0, "pool.ntp.org", "time.nist.gov");
}

void loop() {
  delay(30000);
  time_t now = time(nullptr);
  light = LightSensor.readLightLevel();
  pressure = PreTemSensor.readPressure();
  temperature = PreTemSensor.readTemperature();
  humidity = HumiditySensor.readHumidity();
  console_print(now);
  insert_mod();
  Serial.println(INSERT_SQL);
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
  devmac = WiFi.macAddress();
  devmac.toCharArray(mac, devmac.length()+1);

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
  String text = "Y:" + String(timeinfo->tm_year+1900) + " M:" + String(timeinfo->tm_mon+1) + " D:" \
                + String(timeinfo->tm_mday) + "  " + String(timeinfo->tm_hour) + ":" \
                + String(timeinfo->tm_min) + ":" + String(timeinfo->tm_sec);
  Serial.println(text);
}

void console_print(time_t t) {
  getTextTime(t);
  Serial.println("-------------------------");
  Serial.print("LIGHT:\t\t");
  Serial.println(light);
  Serial.print("PRESSURE:\t");
  Serial.println(pressure);
  Serial.print("TEMPERATURE:\t");
  Serial.println(temperature);
  Serial.print("HUMIDITY:\t");
  Serial.println(humidity);
  Serial.println("=========================");
}

void insert_mod() {
  sprintf(INSERT_val, "( %s, %.2f, %.2f, %.2f, %.2f )", mac, light, pressure, temperature, humidity);
  sprintf(INSERT_SQL, "%s %s", INSERT_syn, INSERT_val);
}