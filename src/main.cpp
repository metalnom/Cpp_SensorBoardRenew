#include "main.hpp"

void setup() {
  Serial.begin(9600);
  Wire.begin();
  LightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  PreTemSensor.begin();
  HumiditySensor.begin();
  wifi_set();
  delay(10000); // 디바이스 등록메뉴에서 쓰레기값을 읽어오지 않도록 일정 시간을 줌
  Serial.print(mac);
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
  // Serial.println(INSERT_SQL);
  cursor->execute(INSERT_SQL); 
}


// 함수 정의부

void wifi_set(void) {
  // Serial.printf("\nConnecting to %s", ssid);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  devmac = WiFi.macAddress();
  devmac.toCharArray(mac, devmac.length()+1);
  
  conn.connect(server_addr, 3306, user, password);
  cursor = new MySQL_Cursor(&conn);  
}

void getTime(time_t now) {
  struct tm *timeinfo;
  timeinfo = localtime(&now);
  year = timeinfo->tm_year + 1900;
  month = timeinfo->tm_mon + 1;
  day = timeinfo->tm_mday;
  t_hour = timeinfo->tm_hour;
  t_min = timeinfo->tm_min;
  t_sec = timeinfo->tm_sec;
}

void insert_mod() {
  sprintf(INSERT_val, "( '%s', %.2f, %.2f, %.2f, %.2f, %d, %d, %d, %d, %d, %d )", \ 
          mac, light, pressure, temperature, humidity, year, month, day, t_hour, t_min, t_sec);
  sprintf(INSERT_SQL, "%s %s", INSERT_syn, INSERT_val);
}