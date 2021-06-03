/*
* This example will demonstrate how to use external TFT_eSPI and TTGO_Watch_Library in combination.
* This method only applies to platfromio IDE, not Arduino IDE
* Copyright 2020 Lewis he
*/

#include "config.h"

TFT_eSPI tft = TFT_eSPI();

TTGOClass *watch = nullptr;

uint32_t targetTime = 0;
uint8_t red = 31;
uint32_t colour = red << 11;
uint8_t green = 0;
uint8_t blue = 0;
uint8_t state = 0;

void setup()
{
    Serial.begin(115200);

    watch = TTGOClass::getWatch();

    //Pass parameters to TTGO_Watch_Library
    watch->setTftExternal(tft);

    watch->begin();

    watch->openBL();

    targetTime = millis() + 1000;
}


void loop()
{
    if (targetTime < millis()) {
        targetTime = millis() + 10000;

        // Colour changing state machine
        for (int i = 0; i < 240; i++) {
            tft.drawFastVLine(i, 0, tft.height(), colour);
            switch (state) {
            case 0:
                green += 2;
                if (green == 64) {
                    green = 63;
                    state = 1;
                }
                break;
            case 1:
                red--;
                if (red == 255) {
                    red = 0;
                    state = 2;
                }
                break;
            case 2:
                blue ++;
                if (blue == 32) {
                    blue = 31;
                    state = 3;
                }
                break;
            case 3:
                green -= 2;
                if (green == 255) {
                    green = 0;
                    state = 4;
                }
                break;
            case 4:
                red ++;
                if (red == 32) {
                    red = 31;
                    state = 5;
                }
                break;
            case 5:
                blue --;
                if (blue == 255) {
                    blue = 0;
                    state = 0;
                }
                break;
            }
            colour = red << 11 | green << 5 | blue;
        }

        // The standard ADAFruit font still works as before
        tft.setTextColor(TFT_BLACK);
        tft.setCursor (12, 5);
        tft.print("Original ADAfruit font!");

        // The new larger fonts do not use the .setCursor call, coords are embedded
        tft.setTextColor(TFT_BLACK, TFT_BLACK); // Do not plot the background colour

        // Overlay the black text on top of the rainbow plot (the advantage of not drawing the backgorund colour!)
        tft.drawCentreString("Font size 2", 80, 14, 2); // Draw text centre at position 80, 12 using font 2

        //tft.drawCentreString("Font size 2",81,12,2); // Draw text centre at position 80, 12 using font 2

        tft.drawCentreString("Font size 4", 80, 30, 4); // Draw text centre at position 80, 24 using font 4

        tft.drawCentreString("12.34", 80, 54, 6); // Draw text centre at position 80, 24 using font 6

        tft.drawCentreString("12.34 is in font size 6", 80, 92, 2); // Draw text centre at position 80, 90 using font 2

        // Note the x position is the top left of the font!

        // draw a floating point number
        float pi = 3.14159; // Value to print
        int precision = 3;  // Number of digits after decimal point
        int xpos = 50;      // x position
        int ypos = 110;     // y position
        int font = 2;       // font number only 2,4,6,7 valid. Font 6 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 0 : a p m
        xpos += tft.drawFloat(pi, precision, xpos, ypos, font); // Draw rounded number and return new xpos delta for next print position
        tft.drawString(" is pi", xpos, ypos, font); // Continue printing from new x position
    }
}