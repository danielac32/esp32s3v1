
#include "SDCARD.h"

SDCARD::SDCARD(uint8_t clk, uint8_t cmd, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
  : _clk(clk),
    _cmd(cmd),
    _d0(d0),
    _d1(d1),
    _d2(d2),
    _d3(d3)
{
    SD_MMC.setPins(clk, cmd, d0, d1, d2, d3);
}

void SDCARD::begin(){
  
  if (!SD_MMC.begin("/sdcard",true)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD_MMC card attached");
    return;
  }

  Serial.print("SD_MMC Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

  //listDir(SD_MMC, "/", 0);
  Serial.printf("Total space: %lluMB\n", SD_MMC.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD_MMC.usedBytes() / (1024 * 1024));

}