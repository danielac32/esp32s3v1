#pragma once

#include <Arduino.h>
#include <SD_MMC.h>
#include <USB.h>
#include <USBMSC.h>

class USBMassStorage {
public:
    USBMassStorage(
        uint8_t clk,
        uint8_t cmd,
        uint8_t d0,
        uint8_t d1,
        uint8_t d2,
        uint8_t d3,
        bool oneBitMode = true
    );

    void begin();

private:
    uint8_t _clk;
    uint8_t _cmd;
    uint8_t _d0;
    uint8_t _d1;
    uint8_t _d2;
    uint8_t _d3;
    bool _oneBitMode;

    USBMSC msc;

    static int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize);
    static int32_t onRead(uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize);
    static bool onStartStop(uint8_t power_condition, bool start, bool load_eject);
    static void usbEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};