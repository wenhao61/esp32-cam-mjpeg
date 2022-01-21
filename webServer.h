#ifndef WEB_SERVER_H_
#define WEB_SERVER_H_
#include <Arduino.h>

void webInit();
String sendCapturedImage2LineNotify(String token);
void listSerialPort(void);
#endif

