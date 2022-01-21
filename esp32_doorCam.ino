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



void loop()
{
  int sensorVal = digitalRead(PIR_PIN);
  currentTime = millis();
  if(sensorVal == LOW && (currentTime - lastTime) > LineTimeval )
  {
    lastTime = currentTime;
    sendCapturedImage2LineNotify("hello");
  }

    listSerialPort();

  server.handleClient();
  delay(50);
}
