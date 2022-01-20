#ifndef HAL_CAMERA_H_
#define HAL_CAMERA_H_
#include <Arduino.h>
#include "src/OV2640.h"

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

void camInit();
#endif

