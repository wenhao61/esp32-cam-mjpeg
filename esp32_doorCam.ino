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


void setup()
{
  Serial.begin(115200);
  //while (!Serial);            //wait for serial connection.

   camInit();
   webInit();
}

void loop()
{
  server.handleClient();
}
