
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

Memory SDCARD::loadRiscvKernel(char* filename){
    Memory mem;
    mem.p = NULL;
    mem.size = 0;

    Serial.printf("Abriendo archivo: %s\n", filename);

    fs::File file = SD_MMC.open(filename);
    if (!file) {
        Serial.println("Error: No se pudo abrir el archivo.");
        return mem; // Retorna con valores nulos
    }

    if (file.isDirectory()) {
        Serial.println("Error: Es un directorio, no un archivo.");
        file.close();
        return mem;
    }

    mem.size = file.size(); // Aquí usamos .size() del fs::File
    mem.p = (char*)ps_malloc(DRAM_SIZE); // Reservamos espacio fijo

    if (!mem.p) {
        Serial.println("Error: No se pudo asignar memoria.");
        file.close();
        mem.size = 0;
        return mem;
    }

    size_t bytes_read = file.readBytes(mem.p, mem.size);
    file.close();

    if (bytes_read != mem.size) {
        Serial.println("Error: Lectura incompleta del archivo.");
        free(mem.p);
        mem.p = NULL;
        mem.size = 0;
    } else {
        Serial.printf("Archivo cargado correctamente (%u bytes)\n", mem.size);
    }

    return mem;
}
 

void SDCARD::free_memory(Memory *mem) {
    if (mem->p) {
        free(mem->p);
        mem->p = NULL;
        mem->size = 0;
    }
}