#include "main.hpp"

void setup() {
  Serial.begin(9600);
  Wire.begin();
  LightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  PreTemSensor.begin();
  HumiditySensor.begin();
  wifi_set();
  delay(10000); // 디바이스 등록메뉴에서 쓰레기값을 읽어오지 않도록 일정 시간을 줌
  Serial.println(mac);
  configTime(9 * 3600, 0, "pool.ntp.org", "time.nist.gov");
}

void loop() {
  delay(30000);     // 데이터베이스 업데이트 주기
  time_t now = time(nullptr);
  light = LightSensor.readLightLevel();
  pressure = PreTemSensor.readPressure();
  temperature = PreTemSensor.readTemperature();
  humidity = HumiditySensor.readHumidity();
  getTime(now);
  insert_mod();
  cursor->execute(INSERT_SQL); 
}


// 함수 정의부

void wifi_set(void) {
  Serial.printf("\nConnecting to %s", ssid);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Serial.print(".");
  }
  // Serial.println("\nConnected to network");
  // Serial.print("My IP address is: ");
  // Serial.println(WiFi.localIP());
  devmac = WiFi.macAddress();
  devmac.toCharArray(mac, devmac.length()+1);
  
  conn.connect(server_addr, 3306, user, password);

  // Serial.print("Connecting to SQL...  ");
  // if (conn.connect(server_addr, 3306, user, password))
    // Serial.println("OK.");
  // else
    // Serial.println("FAILED.");
  
  cursor = new MySQL_Cursor(&conn);  
}

void getTime(time_t now) {
  struct tm *timeinfo;
  timeinfo = localtime(&now);
  year = timeinfo->tm_year + 1900;
  month = timeinfo->tm_mon + 1;
  day = timeinfo->tm_mday;
  // String text = "Y:" + String(timeinfo->tm_year+1900) + " M:" + String(timeinfo->tm_mon+1) + " D:" \
                // + String(timeinfo->tm_mday) + "  " + String(timeinfo->tm_hour) + ":" \
                // + String(timeinfo->tm_min) + ":" + String(timeinfo->tm_sec);
  // Serial.println(text);
}

void console_print(time_t t) {
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
  sprintf(INSERT_val, "( '%s', %.2f, %.2f, %.2f, %.2f, %d, %d, %d )", mac, light, pressure, temperature, humidity, year, month, day);
  sprintf(INSERT_SQL, "%s %s", INSERT_syn, INSERT_val);
  // Serial.println(INSERT_SQL);
}