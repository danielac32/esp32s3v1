
#include <src/SystemCore.h>
//#include "src/peripherals/ST7735/ST7735.h"
//#include "src/peripherals/ETHERNET/W5500.h"
SystemCore core;
ST7735& _display = core._display;


/*
#define TFT_CS   21   // Chip Select
#define TFT_DC   16   // Data/Command
#define TFT_RST  15    
#define TFT_MOSI 17    
#define TFT_SCLK 18    
 
ST7735 _display(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK);
#define ETH_CS       10
#define ETH_IRQ      14
#define ETH_RST      15
#define ETH_SPI_SCK  12
#define ETH_SPI_MISO 13
#define ETH_SPI_MOSI 11
W55 _w55(ETH_CS,ETH_IRQ,ETH_RST,ETH_SPI_SCK,ETH_SPI_MISO,ETH_SPI_MOSI);
*/

void setup() {
 // Serial.begin(115200);
  core.begin();
 // core._w55.begin();
/*
 pinMode(TFT_RST, OUTPUT);
digitalWrite(TFT_RST, HIGH);
digitalWrite(TFT_RST, LOW);
delay(100);
digitalWrite(TFT_RST, HIGH);
delay(100);
_w55.begin();
  _display.begin();
  */
  
  
  // core._display.fillScreen(Color565(0,0,0));
_display.setTextColor(Color565(255,255,255),Color565(0,0,255));
_display.printf("Hola mundo!\n");
_display.printf("Este es un mensaje\nen varias lineas.\n");
_display.printf("Pantalla: %dx%d\n", TFT_WIDTH, TFT_HEIGHT);
 delay(2000);
}

void loop() {
  /*core._display.fillScreen(Color565(0,0,0));
  delay(1000);
  core._display.fillScreen(Color565(0,255,0));
  delay(1000);*/
  
  /*
    _display.fillScreen(Color565(0,0,0));
    _display.fillRect(0,0,127,50,Color565(255,0,0));
    _display.setTextColor(Color565(255,255,255),Color565(255,00,00));
    _display.setCursor(55,20);
    _display.print("red");
    _display.fillRect(0,50,127,100,Color565(0,255,0));
    _display.setTextColor(Color565(255,255,255),Color565(0,255,00));
    _display.setCursor(50,70);
    _display.print("green");
    _display.fillRect(0,100,127,159,Color565(0,0,255));
    _display.setTextColor(Color565(255,255,255),Color565(0,00,255));
    _display.setCursor(55,120);
    _display.print("blue");
    
    _display.drawRect(5,5,118,150,Color565(255,255,255));
    
    delay(5000);
 
    _display.fillScreen(Color565(0,0,0));

    for (uint8_t y=0; y<160; y+=8)
    {
      _display.drawLine(0, 0, 127, y, Color565(255,0,0));  
      _display.drawLine(0, 159, 127, y, Color565(0,255,0));
      _display.drawLine(127, 0, 0, y, Color565(0,0,255));
      _display.drawLine(127, 159, 0, y, Color565(255,255,255));
    }
    delay(5000);
    
    // TEXT   
    _display.fillScreen(Color565(0,0,0));
    _display.setCursor(0,0);
    _display.setTextWrap(1);

    _display.setTextColor(Color565(255,255,255),Color565(0,0,255));
    _display.print("All available chars:\n\n");
    _display.setTextColor(Color565(200,200,255),Color565(50,50,50));
    unsigned char i;
    char ff[]="a";
    for (i=32; i<128; i++) 
    {
      ff[0]=i;
      _display.print(ff);
    }
    
    delay(5000);
*/

delay(1);
}
