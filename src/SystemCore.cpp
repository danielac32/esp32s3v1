

#include "SystemCore.h"
//#include "peripherals/PSRAM/PSRAM.h"
//#include "peripherals/ST7735/ST7735.h"

// _display(PinConfig::TFT_CS, PinConfig::TFT_DC, PinConfig::TFT_RST, 
     //        PinConfig::TFT_MOSI, PinConfig::TFT_SCLK),
 
SystemCore::SystemCore() : 
    _sram(),  // PSRAM no necesita pines (se gestiona por hardware)
    _bootMode(NORMAL),
    _w55(PinConfig::ETH_CS,PinConfig::ETH_IRQ,PinConfig::ETH_RST,PinConfig::ETH_SCK,PinConfig::ETH_MISO,PinConfig::ETH_MOSI),
    _sdcard(PinConfig::SD_CLK,PinConfig::SD_CMD,PinConfig::SD_DATA0,PinConfig::SD_DATA1,PinConfig::SD_DATA2,PinConfig::SD_DATA3),
    _usbmass(PinConfig::SD_CLK,PinConfig::SD_CMD,PinConfig::SD_DATA0,PinConfig::SD_DATA1,PinConfig::SD_DATA2,PinConfig::SD_DATA3,true),
    _display(PinConfig::TFT_CS, PinConfig::TFT_DC, PinConfig::TFT_RST,PinConfig::TFT_MOSI, PinConfig::TFT_SCLK){
    // Aquí puedes añadir más inicializaciones simples si es necesario
}


void SystemCore::begin() {
	Serial.begin(115200);
	pinMode(PinConfig::BOOT_BUTTON,INPUT_PULLUP);
	/*if(!digitalRead(PinConfig::BOOT_BUTTON)){
		Serial.printf("mass storage activated\n" );
		//_usbmass.begin();
	}*/
	
    

    unsigned long start = millis();
	while (millis() - start < 1000) {
		if (digitalRead(PinConfig::BOOT_BUTTON) == LOW) {
		  delay(50);  // antirrebote
		  if (digitalRead(PinConfig::BOOT_BUTTON) == LOW) {
		    Serial.println("Botón pulsado: Iniciando mass storage");
		    delay(1000);
		    _usbmass.begin();
		  }
		}
	}
	//_usbmass.begin();
	/**/

    pinMode(PinConfig::TFT_RST, OUTPUT);
    digitalWrite(PinConfig::TFT_RST, HIGH);
    digitalWrite(PinConfig::TFT_RST, LOW);
    delay(100);
    digitalWrite(PinConfig::TFT_RST, HIGH);
    delay(100);

    _w55.begin();
    delay(100);
    _display.begin();
    delay(100);


    _sdcard.begin();
     delay(100);
    _sram.begin(); 
     delay(100);

      
}
 
 

void SystemCore::run() {
 
}