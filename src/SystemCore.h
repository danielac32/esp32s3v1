#pragma once
#include <Arduino.h>
#include "peripherals/PSRAM/PSRAM.h"  // Incluye la clase PSRAM
#include "peripherals/ST7735/ST7735.h" // Incluye la clase ST7735
#include "peripherals/ETHERNET/W5500.h"
#include "peripherals/SDCARD/SDCARD.h"
#include "peripherals/USBMASS/USBMassStorage.h"

class SystemCore {
public:
    enum BootMode {NORMAL, USB_MASS_STORAGE};

    SystemCore();
    void begin();
    void run();

    // Accesores
    //DisplayManager& getDisplay() { return _display; }
    //SDCardManager& getSDCard() { return _sdCard; }
    // ... otros accesores
    ST7735 _display; 
    SDCARD _sdcard;
    W55 _w55;

private:
    PSRAM _sram;       // Gestiona la PSRAM
    //ST7735 _display;   // Gestiona la pantalla
    BootMode _bootMode; // Modo de arranque
    //W55 _w55;
    //SDCARD _sdcard;
    USBMassStorage _usbmass;


    // Configuración de pines (basada en tu código)
    struct PinConfig {
        static constexpr uint8_t TFT_CS = 21;
        static constexpr uint8_t TFT_DC = 16;
        static constexpr uint8_t TFT_RST = 15;
        static constexpr uint8_t TFT_MOSI = 17;
        static constexpr uint8_t TFT_SCLK = 18;
        
        static constexpr uint8_t SD_CLK = 36;
        static constexpr uint8_t SD_CMD = 35;
        static constexpr uint8_t SD_DATA0 = 37;
        static constexpr uint8_t SD_DATA1 = 38;
        static constexpr uint8_t SD_DATA2 = 33;
        static constexpr uint8_t SD_DATA3 = 34;
        
        static constexpr uint8_t ETH_CS = 10;
        static constexpr uint8_t ETH_IRQ = 14;
        static constexpr uint8_t ETH_RST = 15;//15;
        static constexpr uint8_t ETH_SCK = 12;
        static constexpr uint8_t ETH_MISO = 13;
        static constexpr uint8_t ETH_MOSI = 11;
        
        static constexpr uint8_t BOOT_BUTTON = 0;
    };
};