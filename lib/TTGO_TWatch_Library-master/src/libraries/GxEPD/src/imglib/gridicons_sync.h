#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif
// 24 x 24 gridicons_sync
const unsigned char gridicons_sync[] PROGMEM = { /* 0X01,0X01,0XB4,0X00,0X40,0X00, */
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0xFF, 0xFE, 
0x0F, 0xFF, 0xFC, 0x3F, 0xCF, 0xF8, 0xFF, 0x87, 
0xF9, 0xFF, 0x03, 0xF1, 0xFE, 0x01, 0xF3, 0xFE, 
0x49, 0xF3, 0xFE, 0xCD, 0xB3, 0x7F, 0xCF, 0x92, 
0x7F, 0xCF, 0x80, 0x7F, 0x8F, 0xC0, 0xFF, 0x9F, 
0xE1, 0xFF, 0x1F, 0xF3, 0xFC, 0x3F, 0xFF, 0xF0, 
0x7F, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
