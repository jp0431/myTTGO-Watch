// => Hardware select
// #define LILYGO_WATCH_2019_WITH_TOUCH         // To use T-Watch2019 with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_NO_TOUCH           // To use T-Watch2019 Not touchscreen , please uncomment this line
//#define LILYGO_WATCH_2020_V1                  // To use T-Watch2020 , please uncomment this line

// #define LILYGO_LILYPI_V1                     //LILYPI / TBLOCK requires an external display module
// #define LILYGO_WATCH_BLOCK                   //LILYPI / TBLOCK requires an external display module

#if defined(LILYGO_LILYPI_V1) || defined(LILYGO_WATCH_BLOCK)
#define LILYGO_BLOCK_ST7796S_MODULE          //Use ST7796S
#endif

#include <LilyGoWatch.h>

