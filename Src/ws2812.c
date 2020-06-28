/*
  *****************************************************************************
  * @file    ws2812.c
  * @author  
  * @brief   Source file of ws2812 Controller using SPI.
  *****************************************************************************
*/

#include <stdint.h>
#include "ws2812.h"
#include "stm32f0xx_hal.h"


extern SPI_HandleTypeDef hspi1;
// 24 bit for every pixel and every bit is expanded to 3 bit
static uint8_t WS2812_Buffer[NUM_PIXELS * 24 * 3 / 8];
// According to datasheet > 50 us delay at the end of data frame.
static uint8_t WS2812_Reset[2] = {0x00, 0x00};

/**
  * @brief  Fills WS2812_Buffer[]
  * 
  * PixelNumber is the number of WS2812 in a row beginning at 0. Try not to
  * connect more than 512 WS2812 pixels in a row because of refresh rate.
  * 
  * GRBColor is a 24-bit (3-bytes) data for one WS2812 pixel only. The first
  * byte is GREEN value, The second byte is RED. The third byte is BLUE.
  */
	
void WS2812_Write(uint8_t PixelNumber, unsigned long GRBColor){
  // 24 bit for every pixel and every bit is expanded to 3 bit
  // Can then output 110 for WS2812B logic '1' -> 0x6
  // and 100 for WS2812B logic '0' -> 0x4
  uint32_t encoding = 0;
  uint8_t TempBuffer[9];
  uint8_t index;
    
  // GREEN byte
  index = 0;
  encoding = 0;
  while (index < 8){
    encoding = encoding << 3;
    if (GRBColor & (1 << 23)){
      encoding |= 0x6;
    }
    else{
      encoding |= 0x4;
    }
    GRBColor = GRBColor << 1;
    index++;
  }    
  TempBuffer[0] = ((encoding >> 16) & 0xff);
  TempBuffer[1] = ((encoding >> 8)  & 0xff);
  TempBuffer[2] = (encoding & 0xff);
    
  // RED byte
  index = 0;
  encoding = 0;
  while (index < 8){
    encoding = encoding << 3;
    if (GRBColor & (1 << 23)){
      encoding |= 0x6;
    }
    else{
      encoding |= 0x4;
    }
    GRBColor = GRBColor << 1;
    index++; 
  }    
  TempBuffer[3] = ((encoding >> 16) & 0xff);
  TempBuffer[4] = ((encoding >> 8)  & 0xff);
  TempBuffer[5] = (encoding & 0xff);
    
  // BLUE byte
  index = 0;
  encoding = 0;
  while (index < 8){
    encoding = encoding << 3;
    if (GRBColor & (1 << 23)){
      encoding |= 0x6;
    }
    else{
      encoding |= 0x4;
    }
    GRBColor = GRBColor << 1;
    index++;     
  }    
  TempBuffer[6] = ((encoding >> 16) & 0xff);
  TempBuffer[7] = ((encoding >> 8)  & 0xff);
  TempBuffer[8] = (encoding & 0xff);

  index = 0;
  while(index < 9){
	WS2812_Buffer[index + PixelNumber * 9] = TempBuffer[index];
	index++;
  }      
}


/**
  * @brief  Convert RGB color format to WS2812 GRB  
  */
uint32_t WS2812_Color(uint8_t red, uint8_t green, uint8_t blue){
  return green << 16 | red << 8 | blue;
}


/**
  * @brief  Sets the desired color to a WS2812 pixel
  * 
  * PixelNumber is the number of WS2812 in a row beginning at 0.
  * 
  * red, green, blue are between 0 and 255 (1 byte each) that gives 
  * 16777216 colors in total.
  */
void WS2812_SetPixelColor(uint8_t PixelNumber, uint8_t red, uint8_t green, uint8_t blue){
  WS2812_Write(PixelNumber, WS2812_Color(red, green, blue));	
}


/**
  * @brief  Send data frame to WS2812 LEDs.
  */
void WS2812_Update(void){
  HAL_SPI_Transmit(&hspi1, WS2812_Buffer, sizeof(WS2812_Buffer), HAL_MAX_DELAY);
  HAL_SPI_Transmit(&hspi1, WS2812_Reset, sizeof(WS2812_Reset), HAL_MAX_DELAY);
}


/**
  * @brief  Test WS2812 LEDs. Sets all leds red then green, blue and white.
  */
void WS2812_Test(void){
  // Set all pixels red
  WS2812_SetPixelColor(0,255,0,0);
  WS2812_SetPixelColor(1,255,0,0);
  WS2812_SetPixelColor(2,255,0,0);
  WS2812_SetPixelColor(3,255,0,0);
  WS2812_SetPixelColor(4,255,0,0);
  WS2812_SetPixelColor(5,255,0,0);
  WS2812_SetPixelColor(6,255,0,0);
  WS2812_SetPixelColor(7,255,0,0);     
  WS2812_SetPixelColor(8,255,0,0);
  WS2812_SetPixelColor(9,255,0,0);
  WS2812_SetPixelColor(10,255,0,0);
  WS2812_SetPixelColor(11,255,0,0);
  WS2812_Update();
  HAL_Delay(500);

  // Set all pixels green
  WS2812_SetPixelColor(0,0,255,0);
  WS2812_SetPixelColor(1,0,255,0);
  WS2812_SetPixelColor(2,0,255,0);
  WS2812_SetPixelColor(3,0,255,0);
  WS2812_SetPixelColor(4,0,255,0);
  WS2812_SetPixelColor(5,0,255,0);
  WS2812_SetPixelColor(6,0,255,0);
  WS2812_SetPixelColor(7,0,255,0);     
  WS2812_SetPixelColor(8,0,255,0);
  WS2812_SetPixelColor(9,0,255,0);
  WS2812_SetPixelColor(10,0,255,0);
  WS2812_SetPixelColor(11,0,255,0);
  WS2812_Update();
  HAL_Delay(500);

  // Set all pixels blue
  WS2812_SetPixelColor(0,0,0,255);
  WS2812_SetPixelColor(1,0,0,255);
  WS2812_SetPixelColor(2,0,0,255);
  WS2812_SetPixelColor(3,0,0,255);
  WS2812_SetPixelColor(4,0,0,255);
  WS2812_SetPixelColor(5,0,0,255);
  WS2812_SetPixelColor(6,0,0,255);
  WS2812_SetPixelColor(7,0,0,255);     
  WS2812_SetPixelColor(8,0,0,255);
  WS2812_SetPixelColor(9,0,0,255);
  WS2812_SetPixelColor(10,0,0,255);
  WS2812_SetPixelColor(11,0,0,255); 
  WS2812_Update();
  HAL_Delay(500);

  // Set all pixels white
  WS2812_SetPixelColor(0,255,255,255);
  WS2812_SetPixelColor(1,255,255,255);
  WS2812_SetPixelColor(2,255,255,255);
  WS2812_SetPixelColor(3,255,255,255);
  WS2812_SetPixelColor(4,255,255,255);
  WS2812_SetPixelColor(5,255,255,255);
  WS2812_SetPixelColor(6,255,255,255);
  WS2812_SetPixelColor(7,255,255,255);
  WS2812_SetPixelColor(8,255,255,255);       
  WS2812_SetPixelColor(9,255,255,255);
  WS2812_SetPixelColor(10,255,255,255);
  WS2812_SetPixelColor(11,255,255,255);
  WS2812_Update();
  HAL_Delay(500);
}

/**
  * @brief  Dr who TARDIS lamp effect. gradually turns leds on and off.
  */
void WS2812_TARDISEffect(){
  for(uint8_t i = 255; i >= 1; i = i / 2){
    WS2812_SetPixelColor(0,i,i,i);
    WS2812_SetPixelColor(1,i,i,i);
    WS2812_SetPixelColor(2,i,i,i);
    WS2812_SetPixelColor(3,i,i,i);
    WS2812_SetPixelColor(4,i,i,i);
    WS2812_SetPixelColor(5,i,i,i);
    WS2812_SetPixelColor(6,i,i,i);
    WS2812_SetPixelColor(7,i,i,i);
    WS2812_SetPixelColor(8,i,i,i);
    WS2812_SetPixelColor(9,i,i,i);
    WS2812_SetPixelColor(10,i,i,i);
    WS2812_SetPixelColor(11,i,i,i);
    WS2812_Update();
    HAL_Delay(90);
  }

  for(uint16_t i = 1; i < 256; i *= 2){
    WS2812_SetPixelColor(0,i,i,i);
    WS2812_SetPixelColor(1,i,i,i);
    WS2812_SetPixelColor(2,i,i,i);
    WS2812_SetPixelColor(3,i,i,i);
    WS2812_SetPixelColor(4,i,i,i);
    WS2812_SetPixelColor(5,i,i,i);
    WS2812_SetPixelColor(6,i,i,i);
    WS2812_SetPixelColor(7,i,i,i);
    WS2812_SetPixelColor(8,i,i,i);
    WS2812_SetPixelColor(9,i,i,i);
    WS2812_SetPixelColor(10,i,i,i);
    WS2812_SetPixelColor(11,i,i,i);
    WS2812_Update();
    HAL_Delay(90);
  }

}

