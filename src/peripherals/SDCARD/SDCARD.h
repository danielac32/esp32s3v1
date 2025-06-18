



#pragma once
#include <Arduino.h>
#include <SPI.h>
#include "FS.h"
#include "SD_MMC.h"

#define DRAM_SIZE 400000
#define BLOCK_SIZE 32

typedef struct {
    //FILE *file;
    char name[32];
    uint32_t size;
    char *p;
} Memory;

//extern Memory riscv_sram;
extern Memory mem;

class SDCARD{
public:
	SDCARD(uint8_t clk,uint8_t cmd,uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3);
	void begin();
	Memory loadRiscvKernel(char* filename);
 
	void free_memory(Memory *mem);
private:
	uint8_t _clk;
	uint8_t _cmd;
	uint8_t _d0;
	uint8_t _d1;
	uint8_t _d2;
	uint8_t _d3;
};