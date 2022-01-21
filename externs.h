#ifndef EXTERNS_H_
#define EXTERNS_H_
#include <Arduino.h>
#include "src/OV2640.h"
#include <WebServer.h>

extern OV2640 cam;
extern sensor_t *imgSensor;

extern WebServer server;

#endif


