/*
  *****************************************************************************
  * @file    ws2812.h
  * @author  
  * @brief   Header file of ws2812 Controller using SPI.
  *****************************************************************************
*/

#ifndef WS2812_H_ 
#define WS2812_H_

#define NUM_PIXELS 12

void WS2812_SetPixelColor(uint8_t PixelNumber, uint8_t red, uint8_t green, uint8_t blue);
void WS2812_Write(uint8_t PixelNumber, unsigned long GRBColor);
uint32_t WS2812_Color(uint8_t red, uint8_t green, uint8_t blue);
void WS2812_Update(void);
void WS2812_TARDISEffect();
void WS2812_Test(void);

#endif /* WS2812_H_ */