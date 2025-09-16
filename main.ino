#include "BC26init.h"

#define Grove_Water_Sensor PB9
#define Red_LED PB6
#define Green_LED PB4  
#define Blue_LED PB7
#define Buzz PB3

int waterVal;

// IDEASChain MQTT 設定
String MQTT_Server = "iiot.ideaschain.com.tw";
String MQTT_Port_ideaschain = "1883";
String MQTT_Access_token = "5cv0a145YQopnqYoVCqX";
String MQTTtopic = "v1/devices/me/telemetry";

// quiz.us.kg MQTT 設定
String MQTT_Server_quiz = "quiz.us.kg";
String MQTT_Port_quiz = "2553";
String MQTT_User = "123"; 
String MQTT_Password = "1234";
String MQTTtopic_quiz = "waterVal";

String MQTTmessage1 = "";
String MQTTmessage2 = "";
String Sensor1 = "waterVal"; 
String Sensor2 = "flooded";

String MQTTattrestopic = "v1/devices/me/attributes/response/+";
String MQTTattreqtopic = "v1/devices/me/attributes/request/1";
String water_text = "water";
byte mode = 0;  

int highWaterLevel = 3000;
int retryCount = 0;
const int MAX_RETRY = 3;

bool get_ideaschain_data(String attrestopic, String attreqtopic)
{ 
  Sub_Ideaschain(attrestopic); 
  String waterLevel = Get_Publish_MQTT(mode, attreqtopic, "water_text"); 
  if (waterLevel.toInt() != 0) {
        highWaterLevel = waterLevel.toInt();
        Serial.println("水位高度: " + String(highWaterLevel));
        return true; 
  } 
  return false; 
}

// 發送數據到 IDEASChain
bool send_to_ideaschain(String message1, String message2) {
  bool success = false;
  retryCount = 0;
  
  while (!success && retryCount < MAX_RETRY) {
    if (connect_MQTT(MQTT_Server, MQTT_Port_ideaschain, MQTT_Access_token, MQTT_Access_token)) {
      if (Publish_MQTT(MQTTtopic, message1) && 
          (message2.length() == 0 || Publish_MQTT(MQTTtopic, message2))) {
        success = true;
      }
      Close_MQTT();
    }
    if (!success) {
      retryCount++;
      delay(1000);
    }
  }
  return success;
}

// 發送數據到 quiz.us.kg
bool send_to_quiz(String message) {
  bool success = false;
  retryCount = 0;
  
  while (!success && retryCount < MAX_RETRY) {
    if (connect_MQTT(MQTT_Server_quiz, MQTT_Port_quiz, MQTT_User, MQTT_Password)) {
      if (Publish_MQTT(MQTTtopic_quiz, message)) {
        success = true;
      }
      Close_MQTT();
    }
    if (!success) {
      retryCount++;
      delay(1000);
    }
  }
  return success;
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  
  pinMode(Red_LED, OUTPUT);
  pinMode(Green_LED, OUTPUT);
  pinMode(Grove_Water_Sensor, INPUT);
  pinMode(Blue_LED, OUTPUT);
  pinMode(Buzz, OUTPUT);
  
  digitalWrite(Red_LED, HIGH);
  digitalWrite(Green_LED, HIGH);
  digitalWrite(Blue_LED, HIGH);
  digitalWrite(Buzz, LOW);
  
  if (!BC26init()) {
    delay(30000);
    nvic_sys_reset();
  }
  Serial.println("初始化完成....");
}

void handleWaterLevel() {
  if (waterVal > highWaterLevel) {
    Serial.println("High Level");
    digitalWrite(Red_LED, HIGH);
    digitalWrite(Blue_LED, LOW);
    digitalWrite(Green_LED, LOW);    
    digitalWrite(Buzz, HIGH);
    
    String high = "3";
    MQTTmessage1 = "{\"" + Sensor1 + "\":\"" + waterVal + "\"}";
    MQTTmessage2 = "{\"" + Sensor2 + "\":\"" + high + "\"}";
  } 
  else if ((waterVal > 2400) && (waterVal <= highWaterLevel)) {
    Serial.println("Middle Level");
    digitalWrite(Red_LED, LOW);   
    digitalWrite(Blue_LED, HIGH);     
    digitalWrite(Green_LED, LOW);
    digitalWrite(Buzz, LOW);
    
    String middle = "2";
    MQTTmessage1 = "{\"" + Sensor1 + "\":\"" + waterVal + "\"}";
    MQTTmessage2 = "{\"" + Sensor2 + "\":\"" + middle + "\"}";
  } 
  else if ((waterVal > 1000) && (waterVal <= 2400)) {
    Serial.println("Low Level");
    digitalWrite(Red_LED, LOW);
    digitalWrite(Blue_LED, LOW);
    digitalWrite(Green_LED, HIGH);
    digitalWrite(Buzz, LOW);
    
    String low = "1";
    MQTTmessage1 = "{\"" + Sensor1 + "\":\"" + waterVal + "\"}";
    MQTTmessage2 = "{\"" + Sensor2 + "\":\"" + low + "\"}";
  } 
  else if (waterVal <= 1000) {
    Serial.println("NO Water");
    digitalWrite(Red_LED, LOW);
    digitalWrite(Blue_LED, LOW);
    digitalWrite(Green_LED, LOW);
    digitalWrite(Buzz, LOW);
    
    String no = "0";
    MQTTmessage1 = "{\"" + Sensor1 + "\":\"" + waterVal + "\"}";
    MQTTmessage2 = "{\"" + Sensor2 + "\":\"" + no + "\"}";
  }
}

void loop() {
  waterVal = analogRead(PA0);
  Serial.print(waterVal);    
  Serial.print("   ");  

  // 處理水位狀態和準備數據
  handleWaterLevel();

  // 依序發送到兩個服務器
  bool ideaschain_success = send_to_ideaschain(MQTTmessage1, MQTTmessage2);
  bool quiz_success = send_to_quiz(MQTTmessage1);

  // 顯示發送結果
  Serial.println("IDEAS Chain 發送狀態: " + String(ideaschain_success ? "成功" : "失敗"));
  Serial.println("Quiz 發送狀態: " + String(quiz_success ? "成功" : "失敗"));

  // 檢查是否需要重置系統
  if (Rset_Count > 20) {
    delay(10000);
    nvic_sys_reset();
  }

  delay(500);
}
