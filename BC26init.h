#include <ArduinoJson.h>

byte Rset_Count=0;  //重新啟動系統的計時器   

int waitingTime = 30000;  //等待30秒以回應

String Check_RevData()  //讀取並將每個字符數據轉換為字符串     
{
  String data= "";
  char c;
  while(Serial1.available())
  {
    delay(50);
    c = Serial1.read();     
    data+=c;                
    if (c=='\n') break;
  }
  data.trim();              
  return data;              
}

byte Send_ATcommand(String msg, byte stepnum) //發送AT指令，等待特定回覆
{
  String Showmsg, C_temp;
  Serial.println(msg);
  Serial1.println(msg);
  Showmsg=Check_RevData();
  //Serial.println(Showmsg);

  long StartTime=millis();
  switch (stepnum)
  {
    case 0:                 
         C_temp = "+IP:"; //重置 BC26 模組 Reset BC26 module
         break;
    case 1:                 
         C_temp = "OK"; 
         break;
    case 2:                 
         C_temp = "+CGPADDR:";  //檢查IP
         break;
    case 10:                
         C_temp = "+QMTOPEN: 0,0";  //建立MQTT服務器
         break;
    case 11:                
         C_temp = "+QMTCONN: 0,0,0";  //通過用戶名和密碼連接MQTT服務器
         break;
    case 12:                
         C_temp = "+QMTPUB: 0,0,0";  //發佈MQTT數據
         break;
    case 13:                
         C_temp = "+QMTSUB: 0,1,0,0"; //訂閱MQTT數據
         break;
  }
  while (!Showmsg.startsWith(C_temp))
  {
    Showmsg = Check_RevData();
    if(Showmsg.startsWith("+")) Serial.println(Showmsg);
    if((StartTime + waitingTime) < millis()) return stepnum;
  }
  return 99;
}

bool BC26init() //初始化BC26模組
{
  Send_ATcommand("AT+QGACT=1,1,\"apn\",\"internet.iot\"", 1); 
  Send_ATcommand("AT+QCGDEFCOUNT=\"IP\",\"internet.iot\"", 1);
  Send_ATcommand("AT+QBAND=1,8", 1);
  Send_ATcommand("AT+QRST=1", 0);
//  if (Send_ATcommand("ATE0", 1) == 99)
//  if(Send_ATcommand("AT+CGPADDR=1", 2) == 99) return true;
//  return false;
}

bool connect_MQTT(String Serverx, String port, String user, String pass)  //建立MQTT連接
{
  String S_temp;
  S_temp = "\""+Serverx+"\""+","+port;
  S_temp = "AT+QMTOPEN=0,"+S_temp;
  if (Send_ATcommand(S_temp, 10) != 99) return false;
  S_temp = "\""+user+"\""+","+"\""+pass+"\"";
  S_temp = "AT+QMTCONN=0,0,"+S_temp;
  if (Send_ATcommand(S_temp,11)!=99) return false;
  return true;
}

bool Publish_MQTT(String topic, String message) //發布數據
{
  String S_temp;
  S_temp = "\"" + topic + "\"" + "," + message;
  S_temp = "AT+QMTPUB=0,0,0,0,"+S_temp;
  if (Send_ATcommand(S_temp,12)!=99) return false;
  return true;
}

bool Sub_MQTT(String topic) //訂閱數據
{
  String S_temp;
  S_temp="\""+topic+"\""+","+"0";
  S_temp="AT+QMTSUB=0,1,"+S_temp;
  if(Send_ATcommand(S_temp,13)!=99) return false;
  return true;
}

bool Close_MQTT() //關閉連接
{
  String S_temp;
  S_temp="AT+QMTCLOSE=0";
  if(Send_ATcommand(S_temp,1)!=99) return false;
  return true;
}

String JSON_DEC_data(String input, String findData) //從JSON格式的數據中獲取特定字段
{
  int index=input.indexOf(',');
  int x=input.substring(0, index).toInt();
  String json=input.substring(index+1,input.length());
  index=json.indexOf(':');
  x=json.substring(0,index).toInt();
  json=json.substring(index+1,json.length());
  DynamicJsonDocument doc(1024);從IDEAS Chain平台獲取屬性數據
  deserializeJson(doc,json);
  JsonObject obj=doc.as<JsonObject>();
  return obj[findData];
}

bool Sub_Ideaschain(String attrestopic) //訂閱從IDEAS Chain平台獲取的屬性數據
{
  String S_temp;
  S_temp = "\""+attrestopic +"\""+","+"0";
  S_temp = "AT+QMTSUB=0,1,"+S_temp;
  Serial.println(S_temp);
  Serial1.println(S_temp);
  delay (2000);
  return true;
}

String Get_Publish_MQTT(byte mode, String attreqtopic, String message)  //從IDEAS Chain平台獲取屬性數據
{
  String Showmsg;
  String S_temp, T_temp;
  if (mode==0) T_temp="sharedKeys";
  if (mode==1) T_temp="clientKeys";
  S_temp="\""+attreqtopic+"\""+","+"\"{\""+T_temp+"\":\""+message+"\"}\"";
  S_temp="AT+QMTPUB=0,0,0,0,"+S_temp;
  Serial.println(S_temp);
  Serial1.println(S_temp);
  Showmsg=Check_RevData();
  long StartTime=millis();
  while(!Showmsg.startsWith("+QMTRECV:"))
  {
   delay(1000);
   Showmsg=Check_RevData();
   if(Showmsg.length()>30) break;
   if((StartTime+waitingTime)<millis()) return "error";
  }
  return JSON_DEC_data (Showmsg,message);
}
