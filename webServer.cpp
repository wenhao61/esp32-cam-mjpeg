#include "webServer.h"
#include "externs.h"
#include "homeWifi.h"


#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

WebServer server(80);

const char HEADER[] = "HTTP/1.1 200 OK\r\n" \
                      "Access-Control-Allow-Origin: *\r\n" \
                      "Content-Type: multipart/x-mixed-replace; boundary=123456789000000000000987654321\r\n";
const char BOUNDARY[] = "\r\n--123456789000000000000987654321\r\n";
const char CTNTTYPE[] = "Content-Type: image/jpeg\r\nContent-Length: ";
const int hdrLen = strlen(HEADER);
const int bdrLen = strlen(BOUNDARY);
const int cntLen = strlen(CTNTTYPE);

static bool WebOrLine = 0; // 1:web 2:line 0:idle
String myString;
int res_dly=5;

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
  if(memcmp(sp[0],"line", 4) == 0)
  {
  	sendCapturedImage2LineNotify("hello");
  }
  else if(memcmp(sp[0],"dly", 3) == 0)
  {
  	res_dly = strtol(sp[1],NULL,10);
    if(res_dly < 0)  res_dly = 0;
  }
  else if(memcmp(sp[0],"led", 3) == 0)
  {
    if(memcmp(sp[1],"1", 1) == 0)
	    ledcWrite(4,10);
    else 
        ledcWrite(4,0);
  }
    else if(memcmp(sp[0],"bri", 3) == 0)
    {
        int val = strtol(sp[1],NULL,10);
        if(val >2 || val < -2)
            val = 0;
        imgSensor->set_brightness(imgSensor,val);
    }
    else if(memcmp(sp[0],"sat", 3) == 0)
    {
        int val = strtol(sp[1],NULL,10);
        if(val >2 || val < -2)
            val = 0;
        imgSensor->set_saturation(imgSensor,val);
    }
    else if(memcmp(sp[0],"cst", 3) == 0)
    {
        int val = strtol(sp[1],NULL,10);
        if(val >2 || val < -2)
            val = 0;
        imgSensor->set_contrast(imgSensor,val);
    }

    else if(memcmp(sp[0],"res", 3) == 0)
    {
      if(memcmp(sp[1],"vga", 3) == 0){
        imgSensor->set_framesize(imgSensor, FRAMESIZE_VGA);
        //imgSensor->set_framerate(imgSensor, FRAMERATE_15FPS);
        }
      else if(memcmp(sp[1],"svga", 4) == 0){
        imgSensor->set_framesize(imgSensor, FRAMESIZE_SVGA);
        //imgSensor->set_framerate(imgSensor, FRAMERATE_8FPS);
        }
      else
        {
        imgSensor->set_framesize(imgSensor, FRAMESIZE_QVGA);
        //imgSensor->set_framerate(imgSensor, FRAMERATE_30FPS);
        }
      //cam.setCameraConfig();
    }

}


void listSerialPort(void)
{
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

}

void handle_jpg_stream(void)
{
  char buf[32];
  int s;

  WiFiClient client = server.client();

  client.write(HEADER, hdrLen);
  client.write(BOUNDARY, bdrLen);

  uint8_t *fbBuf = NULL;

  while (true)
  {
    if (!client.connected()) break;
    delay(res_dly);
    listSerialPort();
    
	if(WebOrLine == 2){
		delay(30);
		continue;
	}

	cam.run();
    s = cam.getSize();
    
    uint8_t *ffBuf=cam.getfb();
    client.write(CTNTTYPE, cntLen);
    sprintf( buf, "%d\r\n\r\n", s );
    client.write(buf, strlen(buf));
    client.write((char *)ffBuf, s);    

    client.write(BOUNDARY, bdrLen);
  }
}

const char JHEADER[] = "HTTP/1.1 200 OK\r\n" \
                       "Content-disposition: inline; filename=capture.jpg\r\n" \
                       "Content-type: image/jpeg\r\n\r\n";
const int jhdLen = strlen(JHEADER);

void handle_jpg(void)
{
static int waitCnt=30;
while(WebOrLine == 2)
{
	delay(100);
	if(--waitCnt <= 0)
		return;
}

  WiFiClient client = server.client();

  if (!client.connected()) return;
  cam.run();
  client.write(JHEADER, jhdLen);
  client.write((char *)cam.getfb(), cam.getSize());

Serial.println("handle_jpg successful");

}

void handleNotFound()
{
  String message = "Server is running!\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  server.send(200, "text / plain", message);
}

String sendCapturedImage2LineNotify(String token) {

  WebOrLine = 2;

  token = myLineNotifyToken;

  WiFiClientSecure client_tcp;
  client_tcp.setInsecure();   //run version 1.0.5 or above
  Serial.println("Connect to notify-api.line.me");
  if (client_tcp.connect("notify-api.line.me", 443)) {
    Serial.println("Connection successful");
    
    String message = "ESP32-CAM";
    String head = "--Taiwan\r\nContent-Disposition: form-data; name=\"message\"; \r\n\r\n" + message + "\r\n--Taiwan\r\nContent-Disposition: form-data; name=\"imageFile\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--Taiwan--\r\n";

    cam.run();
    uint8_t *fbBuf = cam.getfb();
	uint16_t imageLen = cam.getSize();
     if(fbBuf)
        Serial.println("Connection successful, image size = "+ String((int)imageLen));
     
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;
   
    client_tcp.println("POST /api/notify HTTP/1.1");
    client_tcp.println("Connection: close"); 
    client_tcp.println("Host: notify-api.line.me");
    client_tcp.println("Authorization: Bearer " + token);
    client_tcp.println("Content-Length: " + String(totalLen));
    client_tcp.println("Content-Type: multipart/form-data; boundary=Taiwan");
    client_tcp.println();
    client_tcp.print(head);
    

	size_t fbLen = (size_t)imageLen;
    
	WebOrLine = 0;

    if(fbLen==0 || fbBuf == NULL)
    {
    	return "Connected to notify-api.line.me failed.";
    }
	
    for (size_t n=0;n<fbLen;n=n+1024) {
      Serial.print("@");
      if (n+1024<fbLen) {
        client_tcp.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen%1024>0) {
        size_t remainder = fbLen%1024;
        client_tcp.write(fbBuf, remainder);
      }
    }  
    Serial.println();
    
    client_tcp.print(tail);
    Serial.println("Connection successful, image size = "+ String((int)fbLen));

    String getResponse="",Feedback="";
    int waitTime = 10000;   // timeout 10 seconds
    long startTime = millis();
    boolean state = false;
    
    while ((startTime + waitTime) > millis()) {
      Serial.print(".");
      delay(100);      
      while (client_tcp.available())  {
          char c = client_tcp.read();
          if (state==true) Feedback += String(c);        
          if (c == '\n') {
            if (getResponse.length()==0) state=true; 
            getResponse = "";
          } 
          else if (c != '\r')
            getResponse += String(c);
          startTime = millis();
       }
       if (Feedback.length()>0) break;
    }
    Serial.println();
    client_tcp.stop();
	
    return Feedback;
  }
  else {
    return "Connected to notify-api.line.me failed.";
  }
}

void webInit()
{
    IPAddress ip;
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID1, PWD1);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(F("."));
    }
    ip = WiFi.localIP();
    Serial.println(F("WiFi connected"));
    Serial.println("");
    Serial.print("Stream Link: http://");
    Serial.print(ip);
    Serial.println("/live");
    server.on("/live", HTTP_GET, handle_jpg_stream);
    server.on("/jpg", HTTP_GET, handle_jpg);
    server.onNotFound(handleNotFound);
    server.begin();

}


