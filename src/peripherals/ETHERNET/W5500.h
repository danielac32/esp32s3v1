

#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <ETH.h>
#include <WiFi.h>

class W55{
public:
	W55(uint8_t cs,uint8_t irq,uint8_t rst,uint8_t sck,uint8_t miso,uint8_t mosi);

	void begin();
private:
	 uint8_t _cs;
	 uint8_t _irq;
	 uint8_t _rst;
	 uint8_t _sck;
	 uint8_t _miso;
	 uint8_t _mosi;
    static void onEvent(arduino_event_id_t event, arduino_event_info_t info);
};