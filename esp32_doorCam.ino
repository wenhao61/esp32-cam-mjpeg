/*
  This is a simple MJPEG streaming webserver implemented for AI-Thinker ESP32-CAM and
  ESP32-EYE modules.
  This is tested to work with VLC and Blynk video widget.

  Inspired by and based on this Instructable: $9 RTSP Video Streamer Using the ESP32-CAM Board
  (https://www.instructables.com/id/9-RTSP-Video-Streamer-Using-the-ESP32-CAM-Board/)

  Board: AI-Thinker ESP32-CAM
*/
/*
Product Name: Wenhao's doorCam
    
Hardware : ESP32CAM
           PIR
           
Software : Arduino
           Line Notify
           Web Server
Use case: 
    1. Trigger with PIR
    2. Capture image using ov2640
    3. after alarm, send the picture to end-user using line notify.
    4. live view and image setting using RESTful API.
*/


#include "halCamera.h"
#include "webServer.h"
#include "externs.h"

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define PIR_PIN (16)
#define LineTimeval (10000)
long lastTime=0, currentTime=0;


void parseCmd(String cmd)
{
  char sp[5][8]={};
  int i=0,j=0, idx=0;
  const char *strCmd = cmd.c_str();
  for(i=0;i<cmd.length();i++)
  {
    if(strCmd[i]==' ')
    {
      j=0;
      idx++;
      if(idx>=5)
        break;
    }
    else{
      if(j<8){
        sp[idx][j++]=strCmd[i];
      }
    } 
  }
  for(i=0;i<=idx;i++)
  {
    Serial.println(sp[i]);
  }
  if(memcmp(sp[0],"help", 4) == 0)
  {
  	sendCapturedImage2LineNotify("hello");
  }
  else if(memcmp(sp[0],"on", 2) == 0)
  {
  //	digitalWrite(4,HIGH);
	ledcWrite(4,10);
  }
    else if(memcmp(sp[0],"off", 3) == 0)
  {
  	//digitalWrite(4,LOW);
	ledcWrite(4,0);
  }
#if 0
  if(String(sp[0]) == "help")
  {
  	sendCapturedImage2LineNotify("hello");
  }
  else if(String(sp[0]) == "led")
  {
  	if(String(sp[1]) == "on")
  		digitalWrite(4,HIGH);
	else
		digitalWrite(4,LOW);
  }	
  else
  {
  	Serial.println("##### " + sp[0]);
  }
#endif
}



void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //關閉電源不穩就重開機的設定
  Serial.begin(115200);
  //while (!Serial);            //wait for serial connection.
   pinMode(PIR_PIN, INPUT_PULLUP);

   camInit();
   webInit();
     //閃光燈(GPIO4)
  ledcAttachPin(4, 4);  
  ledcSetup(4, 5000, 8);

	ledcWrite(4,10);
    delay(1000);
    ledcWrite(4,0);
    delay(1000); 
   lastTime = millis();
}

String myString;

void loop()
{
  int sensorVal = digitalRead(PIR_PIN);
  currentTime = millis();
  if(sensorVal == LOW && (currentTime - lastTime) > LineTimeval )
  {
    lastTime = currentTime;
    sendCapturedImage2LineNotify("hello");
  }

  while (Serial.available()) {
	  char c = Serial.read();
	  if(c!='\n'){
		  myString += c;
	  }
	  delay(5);    // 沒有延遲的話 UART 串口速度會跟不上Arduino的速度，會導致資料不完整
  }
  
  if(myString.length()>0)
  {
	  parseCmd(myString);
	  Serial.println(myString);
	  myString="";
  }

  
  server.handleClient();
  delay(50);
}
