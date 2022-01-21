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

/*
sensor_t * s = esp_camera_sensor_get()
s->set_brightness(s, 0);     // -2 to 2
s->set_contrast(s, 0);       // -2 to 2
s->set_saturation(s, 0);     // -2 to 2
s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
s->set_aec2(s, 0);           // 0 = disable , 1 = enable
s->set_ae_level(s, 0);       // -2 to 2
s->set_aec_value(s, 300);    // 0 to 1200
s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
s->set_agc_gain(s, 0);       // 0 to 30
s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
s->set_bpc(s, 0);            // 0 = disable , 1 = enable
s->set_wpc(s, 1);            // 0 = disable , 1 = enable
s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
s->set_lenc(s, 1);           // 0 = disable , 1 = enable
s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
s->set_vflip(s, 0);          // 0 = disable , 1 = enable
s->set_dcw(s, 1);            // 0 = disable , 1 = enable
s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
s->set_framesize(s, FRAMESIZE_VGA);
typedef enum {
    FRAMESIZE_40x30,    // 40x30
    FRAMESIZE_64x32,    // 64x32
    FRAMESIZE_64x64,    // 64x64
    FRAMESIZE_QQCIF,    // 88x72
    FRAMESIZE_QQVGA,    // 160x120
    FRAMESIZE_QQVGA2,   // 128x160
    FRAMESIZE_QCIF,     // 176x144
    FRAMESIZE_HQVGA,    // 220x160
    FRAMESIZE_QVGA,     // 320x240
    FRAMESIZE_CIF,      // 352x288
    FRAMESIZE_VGA,      // 640x480
    FRAMESIZE_SVGA,     // 800x600
    FRAMESIZE_SXGA,     // 1280x1024
    FRAMESIZE_UXGA,     // 1600x1200
} framesize_t;

typedef enum {
    FRAMERATE_2FPS =0x9F,
    FRAMERATE_8FPS =0x87,
    FRAMERATE_15FPS=0x83,
    FRAMERATE_30FPS=0x81,
    FRAMERATE_60FPS=0x80,
} framerate_t;
int  (*reset)               (sensor_t *sensor);
int  (*set_pixformat)       (sensor_t *sensor, pixformat_t pixformat);
int  (*set_framesize)       (sensor_t *sensor, framesize_t framesize);
int  (*set_framerate)       (sensor_t *sensor, framerate_t framerate);
int  (*set_contrast)        (sensor_t *sensor, int level);
int  (*set_brightness)      (sensor_t *sensor, int level);
int  (*set_saturation)      (sensor_t *sensor, int level);
int  (*set_gainceiling)     (sensor_t *sensor, gainceiling_t gainceiling);
int  (*set_quality)         (sensor_t *sensor, int quality);
int  (*set_colorbar)        (sensor_t *sensor, int enable);
int  (*set_whitebal)        (sensor_t *sensor, int enable);
int  (*set_gain_ctrl)       (sensor_t *sensor, int enable);
int  (*set_exposure_ctrl)   (sensor_t *sensor, int enable);
int  (*set_hmirror)         (sensor_t *sensor, int enable);
int  (*set_vflip)           (sensor_t *sensor, int enable);
int  (*set_special_effect)  (sensor_t *sensor, sde_t sde);


*/

