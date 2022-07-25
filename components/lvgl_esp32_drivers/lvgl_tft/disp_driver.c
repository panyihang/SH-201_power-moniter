/**
 * @file disp_driver.c
 */

#include "disp_driver.h"
#include "disp_spi.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "driver/gpio.h"

#define TFT_RD 33
#define TFT_WR 37
#define TFT_RS 21

#define TFT_CS 34
#define TFT_RST 36

#define TFT_D0 16
#define TFT_D1 15
#define TFT_D2 14
#define TFT_D3 13
#define TFT_D4 12
#define TFT_D5 11
#define TFT_D6 10
#define TFT_D7 9
#define TFT_D8 8
#define TFT_D9 7
#define TFT_D10 6
#define TFT_D11 5
#define TFT_D12 4
#define TFT_D13 3
#define TFT_D14 2
#define TFT_D15 1

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430

#define setxcmd 0x47
#define setycmd 0x45
#define wramcmd 0x22

#define width 320
#define height 480

#define HIGH 1
#define LOW 0

void initGpioOut(uint16_t pinNumber)
{
  gpio_pad_select_gpio(pinNumber);
  gpio_set_direction(pinNumber, GPIO_MODE_OUTPUT);
}

void writeGPIO(uint16_t pinNumber, uint16_t value)
{
  gpio_set_level(pinNumber, value);
}

void delayMs(uint32_t ms)
{
  ets_delay_us(ms);
}

void DATAOUT(uint16_t buffer)
{
  writeGPIO(TFT_D0, (buffer & BIT0) ? HIGH : LOW);
  writeGPIO(TFT_D1, (buffer & BIT1) ? HIGH : LOW);
  writeGPIO(TFT_D2, (buffer & BIT2) ? HIGH : LOW);
  writeGPIO(TFT_D3, (buffer & BIT3) ? HIGH : LOW);
  writeGPIO(TFT_D4, (buffer & BIT4) ? HIGH : LOW);
  writeGPIO(TFT_D5, (buffer & BIT5) ? HIGH : LOW);
  writeGPIO(TFT_D6, (buffer & BIT6) ? HIGH : LOW);
  writeGPIO(TFT_D7, (buffer & BIT7) ? HIGH : LOW);
  writeGPIO(TFT_D8, (buffer & BIT8) ? HIGH : LOW);
  writeGPIO(TFT_D9, (buffer & BIT9) ? HIGH : LOW);
  writeGPIO(TFT_D10, (buffer & BIT10) ? HIGH : LOW);
  writeGPIO(TFT_D11, (buffer & BIT11) ? HIGH : LOW);
  writeGPIO(TFT_D12, (buffer & BIT12) ? HIGH : LOW);
  writeGPIO(TFT_D13, (buffer & BIT13) ? HIGH : LOW);
  writeGPIO(TFT_D14, (buffer & BIT14) ? HIGH : LOW);
  writeGPIO(TFT_D15, (buffer & BIT15) ? HIGH : LOW);
}

void TFT_WR_REG(uint16_t Reg)
{
  writeGPIO(TFT_RS, LOW);
  DATAOUT(Reg);
  writeGPIO(TFT_WR, LOW);
  writeGPIO(TFT_WR, HIGH);
}

void TFT_WR_DATA(uint16_t Data)
{
  writeGPIO(TFT_RS, HIGH);
  DATAOUT(Data);
  writeGPIO(TFT_WR, LOW);
  writeGPIO(TFT_WR, HIGH);
}

void TFT_WriteReg(uint16_t TFT_Reg, uint16_t TFT_RegValue)
{
  TFT_WR_REG(TFT_Reg);
  TFT_WR_DATA(TFT_RegValue);
}

void TFT_Reset(void)
{
  writeGPIO(TFT_RST, HIGH);
  delayMs(50);
  writeGPIO(TFT_RST, LOW);
  delayMs(50);
  writeGPIO(TFT_RST, HIGH);
  delayMs(50);
}

void TFT_PIN_Init()
{
  initGpioOut(TFT_RD);
  initGpioOut(TFT_WR);
  initGpioOut(TFT_RST);
  initGpioOut(TFT_RS);
  initGpioOut(TFT_CS);

  initGpioOut(TFT_D0);
  initGpioOut(TFT_D1);
  initGpioOut(TFT_D2);
  initGpioOut(TFT_D3);
  initGpioOut(TFT_D4);
  initGpioOut(TFT_D5);
  initGpioOut(TFT_D6);
  initGpioOut(TFT_D7);
  initGpioOut(TFT_D8);
  initGpioOut(TFT_D9);
  initGpioOut(TFT_D10);
  initGpioOut(TFT_D11);
  initGpioOut(TFT_D12);
  initGpioOut(TFT_D13);
  initGpioOut(TFT_D14);
  initGpioOut(TFT_D15);

  writeGPIO(TFT_D0, LOW);
  writeGPIO(TFT_D1, LOW);
  writeGPIO(TFT_D2, LOW);
  writeGPIO(TFT_D3, LOW);
  writeGPIO(TFT_D4, LOW);
  writeGPIO(TFT_D5, LOW);
  writeGPIO(TFT_D6, LOW);
  writeGPIO(TFT_D7, LOW);
  writeGPIO(TFT_D8, LOW);
  writeGPIO(TFT_D9, LOW);
  writeGPIO(TFT_D10, LOW);
  writeGPIO(TFT_D11, LOW);
  writeGPIO(TFT_D12, LOW);
  writeGPIO(TFT_D13, LOW);
  writeGPIO(TFT_D14, LOW);
  writeGPIO(TFT_D15, LOW);
}

void TFT_WriteRAM_Prepare(void)
{
  TFT_WR_REG(wramcmd);
}

void TFT_SetCursor(uint16_t x, uint16_t y)
{
  TFT_WriteReg(0x20, y);
  TFT_WriteReg(0x21, x);
  TFT_WriteRAM_Prepare();
}

void TFT_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
  TFT_WriteReg(0x44, xEnd);
  TFT_WriteReg(0x45, xStar);
  TFT_WriteReg(0x46, yEnd);
  TFT_WriteReg(0x47, yStar);
  TFT_WriteReg(0x20, yStar);
  TFT_WriteReg(0x21, xStar);
  TFT_WriteRAM_Prepare();
}

void TFT_FillScreen(uint32_t Color)
{
  uint32_t index = 0;
  TFT_SetWindows(0, 0, width - 1, height - 1);
  writeGPIO(TFT_RS, HIGH);
  for (index = 0; index < width * height; index++)
  {
    TFT_WR_DATA(Color);
  }
}

void TFT_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  TFT_WR_REG(0x2a);
  TFT_WR_DATA(x1 >> 8);
  TFT_WR_DATA(x1 & 0xff);
  TFT_WR_DATA(x2 >> 8);
  TFT_WR_DATA(x2 & 0xff);
  TFT_WR_REG(0x2b);
  TFT_WR_DATA(y1 >> 8);
  TFT_WR_DATA(y1 & 0xff);
  TFT_WR_DATA(y2 >> 8);
  TFT_WR_DATA(y2 & 0xff);
  TFT_WR_REG(0x2c);
}

void TFT_Fill(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t * color)
{
    uint32_t index = 0;
    TFT_SetWindows(xStart, yStart, xEnd, yEnd);
    writeGPIO(TFT_RS, 1);
    for (index = 0; index < (xEnd - xStart + 1) * (yEnd - yStart + 1); index++)
    {
        TFT_WR_DATA(*color);
        color++;
    }
}

void TFT_Init()
{
  TFT_PIN_Init();
  TFT_Reset();

  writeGPIO(TFT_RD, HIGH);

  delayMs(50);
  //************* Start Initial Sequence **********//
  TFT_WriteReg(0X0001, 0X013C);
  TFT_WriteReg(0X0002, 0X0100);
  TFT_WriteReg(0X0003, 0X0030);
  //1001010010000
  TFT_WriteReg(0X0005, 0X0000);
  TFT_WriteReg(0X0008, 0X0808);
  TFT_WriteReg(0X000A, 0X0500);
  TFT_WriteReg(0X000B, 0X0000);
  TFT_WriteReg(0X000C, 0X0770);
  TFT_WriteReg(0X000D, 0X0000);
  TFT_WriteReg(0X000E, 0X0001);

  TFT_WriteReg(0X0011, 0X0406);
  TFT_WriteReg(0X0012, 0X000E);
  TFT_WriteReg(0X0013, 0X0222);
  TFT_WriteReg(0X0014, 0X001C);
  TFT_WriteReg(0X0015, 0X3679);
  TFT_WriteReg(0X0016, 0X0000);

  TFT_WriteReg(0X0030, 0X6A50);
  TFT_WriteReg(0X0031, 0X00C9);
  TFT_WriteReg(0X0032, 0XC7BE);
  TFT_WriteReg(0X0033, 0X0003);
  TFT_WriteReg(0X0036, 0X3443);
  TFT_WriteReg(0X003B, 0X0000);
  TFT_WriteReg(0X003C, 0X0000);

  TFT_WriteReg(0X002C, 0X6A50);
  TFT_WriteReg(0X002D, 0X00C9);
  TFT_WriteReg(0X002E, 0XC7BE);
  TFT_WriteReg(0X002F, 0X0003);
  TFT_WriteReg(0X0035, 0X3443);
  TFT_WriteReg(0X0039, 0X0000);
  TFT_WriteReg(0X003A, 0X0000);

  TFT_WriteReg(0X0028, 0X6A50);
  TFT_WriteReg(0X0029, 0X00C9);
  TFT_WriteReg(0X002A, 0XC7BE);
  TFT_WriteReg(0X002B, 0X0003);
  TFT_WriteReg(0X0034, 0X3443);
  TFT_WriteReg(0X0037, 0X0000);
  TFT_WriteReg(0X0038, 0X0000);

  TFT_WriteReg(0X0040, 0X0000);
  TFT_WriteReg(0X0041, 0X01DF);
  TFT_WriteReg(0X0042, 0X0000);
  TFT_WriteReg(0X0043, 0X0000);

  delayMs(20);

  TFT_WriteReg(0X0012, 0X200E);

  delayMs(160);

  TFT_WriteReg(0X0012, 0X2003);

  delayMs(40);

  TFT_WriteReg(0X0007, 0X0012);
  delayMs(40);
  TFT_WriteReg(0X0007, 0X0017);

  delayMs(100);
  TFT_WR_REG(0x29);
  TFT_WriteReg(0x36, 0x70);
    initGpioOut(35);
  writeGPIO(35, HIGH);
}

void disp_driver_init(void)
{
    TFT_Init();
}

void disp_driver_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
    TFT_Fill(area->x1, area->y1, area->x2, area->y2, (uint16_t *)color_map);
    lv_disp_flush_ready(drv);
}

void disp_driver_rounder(lv_disp_drv_t * disp_drv, lv_area_t * area)
{

}

void disp_driver_set_px(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
    lv_color_t color, lv_opa_t opa) 
{

}
