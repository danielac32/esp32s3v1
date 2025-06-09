



#pragma once
#include <Arduino.h>
#include <SPI.h>
#include "FS.h"
#include "SD_MMC.h"

class SDCARD{
public:
	SDCARD(uint8_t clk,uint8_t cmd,uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3);
	void begin();
private:
	uint8_t _clk;
	uint8_t _cmd;
	uint8_t _d0;
	uint8_t _d1;
	uint8_t _d2;
	uint8_t _d3;
};