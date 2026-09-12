#ifndef _DISPLAY_EPD_W21_SPI_
#define _DISPLAY_EPD_W21_SPI_

#include "DEV_Config.h"

//IO settings
#define isEPD_W21_BUSY DEV_Digital_Read(EPD_BUSY_PIN)  //BUSY
#define EPD_W21_RST_0 DEV_Digital_Write(EPD_RST_PIN, 0)  //RES
#define EPD_W21_RST_1 DEV_Digital_Write(EPD_RST_PIN, 1)
#define EPD_W21_DC_0  DEV_Digital_Write(EPD_DC_PIN, 0) //DC
#define EPD_W21_DC_1  DEV_Digital_Write(EPD_DC_PIN, 1)
#define EPD_W21_CS_0 DEV_Digital_Write(EPD_CS_PIN, 0) //CS
#define EPD_W21_CS_1 DEV_Digital_Write(EPD_CS_PIN, 1)


void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char datas);
void EPD_W21_WriteCMD(unsigned char command);


#endif
