#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif
// 24 x 24 gridicons_pause
const unsigned char gridicons_pause[] PROGMEM = { /* 0X01,0X01,0XB4,0X00,0X40,0X00, */
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 
0xFF, 0xFE, 0x00, 0x7F, 0xF8, 0x00, 0x1F, 0xF0, 
0x00, 0x0F, 0xF0, 0x00, 0x0F, 0xE0, 0x00, 0x07, 
0xE0, 0x66, 0x07, 0xC0, 0x66, 0x03, 0xC0, 0x66, 
0x03, 0xC0, 0x66, 0x03, 0xC0, 0x66, 0x03, 0xC0, 
0x66, 0x03, 0xC0, 0x66, 0x03, 0xE0, 0x66, 0x07, 
0xE0, 0x00, 0x07, 0xF0, 0x00, 0x0F, 0xF0, 0x00, 
0x0F, 0xF8, 0x00, 0x1F, 0xFE, 0x00, 0x7F, 0xFF, 
0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
