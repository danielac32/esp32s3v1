

#include "PSRAM.h"


PSRAM::PSRAM(){}

void PSRAM::begin(){
	if (psramFound()) {
        Serial.println("PSRAM encontrada y habilitada.");
	    // Mostrar el tamaño total de la PSRAM
	    size_t psramSize = ESP.getPsramSize();
	    Serial.printf("Tamaño total de PSRAM: %d bytes\n", psramSize);

	    // Probar la PSRAM con una asignación dinámica
	    const int bufferSize = 1024 * 1024 * 7.5; // 1 MB
	    if (bufferSize > psramSize) {
	      Serial.println("El tamaño del búfer solicitado excede la PSRAM disponible.");
	      return;
	    }

	    // Asignar memoria en la PSRAM
	    uint8_t *psramBuffer = (uint8_t *)ps_malloc(bufferSize);
	    if (psramBuffer == nullptr) {
	      Serial.println("Fallo al asignar memoria en la PSRAM.");
	      return;
	    }

	    Serial.printf("Memoria asignada correctamente en la PSRAM: %d bytes\n", bufferSize);

	    // Escribir datos en la PSRAM
	    for (int i = 0; i < bufferSize; i++) {
	      psramBuffer[i] = i % 256; // Escribir valores de 0 a 255 repetidamente
	    }
	    Serial.println("Datos escritos en la PSRAM.");

	    // Leer y verificar los datos desde la PSRAM
	    bool testPassed = true;
	    for (int i = 0; i < bufferSize; i++) {
	      if (psramBuffer[i] != (i % 256)) {
	        Serial.printf("Error en la posición %d: esperado %d, obtenido %d\n", i, i % 256, psramBuffer[i]);
	        testPassed = false;
	        break;
	      }
	    }

	    if (testPassed) {
	      Serial.println("Prueba de PSRAM exitosa: todos los datos coinciden.");
	    } else {
	      Serial.println("Prueba de PSRAM fallida.");
	    }

	    // Liberar la memoria asignada
	    free(psramBuffer);
	    Serial.println("Memoria liberada.");
  } else {
    Serial.println("PSRAM no encontrada o no habilitada.");
    while(1);
  }
}