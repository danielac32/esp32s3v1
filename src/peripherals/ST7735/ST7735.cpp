#include "ST7735.h"
 

#include <SPI.h>
SPIClass hspi(HSPI);  // Usa el SPI hardware (HSPI)

// Constructor: recibe pines personalizados
ST7735::ST7735(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t sclk)
  : _cs(cs), _dc(dc), _rst(rst), _mosi(mosi), 
   _sclk(sclk),cursor_x(0), cursor_y(0),
   textcolor(Color565(255,255,255)), textbgcolor(Color565(0,0,0)),wrap(true)
   {
    hspi.begin(sclk, -1, mosi, -1);  // Inicia SPI (sin MISO)
    hspi.setFrequency(80 * 1000000);   // Frecuencia alta (40 MHz)
    //pinMode(_sclk, OUTPUT);
  //pinMode(_mosi, OUTPUT);
  pinMode(_cs,   OUTPUT);
  pinMode(_dc,   OUTPUT);

  digitalWrite(_cs, HIGH);
  //digitalWrite(_sclk, HIGH); // Modo SPI_IDLE_HIGH
    }





/* SPI general support functions */

void ST7735::spi_sw_send(uint8_t data) {
  digitalWrite(_cs, LOW);

  /*for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(_sclk, LOW);
    digitalWrite(_mosi, (data & 0x80) ? HIGH : LOW);
    data <<= 1;
    //delayMicroseconds(1);  // Pequeño delay para estabilidad
    digitalWrite(_sclk, HIGH);
    //delayMicroseconds(1);
  }*/
 hspi.transfer(data);
  digitalWrite(_cs, HIGH);
}



void ST7735::writecommand(uint8_t cmd) {
  digitalWrite(_dc, LOW); // Comando
  spi_sw_send(cmd);
}

void ST7735::writedata(uint8_t data) {
  digitalWrite(_dc, HIGH); // Datos
  spi_sw_send(data);
}

void ST7735::spistreampixel(uint16_t color) {
  spi_sw_send(color >> 8);
  spi_sw_send(color & 0xFF);
  
  
}

void ST7735::setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  writecommand(0x2A); // Column addr set
  writedata(x0);
  writedata(x1);

  writecommand(0x2B); // Row addr set
  writedata(y0);
  writedata(y1);

  writecommand(0x2C); // RAM write
}

void ST7735::myDelay(uint16_t ms) {
  delay(ms);
}

void ST7735::begin() {
 

 /* if (_rst != -1) {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(200);
    digitalWrite(_rst, HIGH);
    delay(150);
  }*/

  // Secuencia básica de inicialización
  writecommand(0x11); // Salir del Sleep
  delay(120);

  writecommand(0xB1); // Frame Rate
  writedata(0x01);
  writedata(0x2C);
  writedata(0x2D);

  writecommand(0xB2); // Frame Rate
  writedata(0x01);
  writedata(0x2C);
  writedata(0x2D);

  writecommand(0xB3);
  writedata(0x01);
  writedata(0x2C);
  writedata(0x2D);
  writedata(0x2C);
  writedata(0x2D);

  writecommand(0xB4); // Inversion Control
  writedata(0x07);

  writecommand(0xC0); // Power Control 1
  writedata(0xA2);
  writedata(0x02);
  writedata(0x84);

  writecommand(0xC1);
  writedata(0xC5);

  writecommand(0xC2);
  writedata(0x0A);
  writedata(0x00);

  writecommand(0xC3);
  writedata(0x8A);
  writecommand(0xC4);
  writedata(0x8A);

  writecommand(0xC5); // VCOM
  writedata(0x0E);

  writecommand(0x36); // Memory Access Control
  writedata(0xC8);     // Rotación

  writecommand(0x3A); // Pixel Format
  writedata(0x05);    // 16-bit/pixel

  writecommand(0x29); // Display on
}


void ST7735::fillScreen(uint16_t color) {
  for (uint16_t y = 0; y < 160; y++) {
    for (uint16_t x = 0; x < 128; x++) {
      setAddrWindow(x, y, x, y);  // Ventana de 1 píxel (como drawPixel)
      digitalWrite(_dc, HIGH);    // Modo datos
      digitalWrite(_cs, LOW);
      spi_sw_send(color >> 8);    // Parte alta
      spi_sw_send(color & 0xFF);  // Parte baja
      digitalWrite(_cs, HIGH);
    }
  }
}

void ST7735::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= 128) || (y < 0) || (y >= 160)) return;
  setAddrWindow(x, y, x+1, y+1);
  digitalWrite(_dc, HIGH);
  spistreampixel(color);
}



void ST7735::printf(const char *format, ...) {
    char buffer[256];  // Buffer temporal
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);  // Formatea el string
    va_end(args);

    int16_t originalX = cursor_x;

    const char *str = buffer;
    while (*str) {
        if (*str == '\n') {
            cursor_y += 8;     // Salta una línea
            cursor_x = originalX;  // Vuelve al inicio horizontal
        } else {
            drawChar(cursor_x, cursor_y, *str, textcolor, textbgcolor);
            cursor_x += 6;     // Ancho promedio de carácter
        }
        str++;
    }
}

/*
void ST7735::flush() {
    setAddrWindow(0, 0, TFT_WIDTH - 1, TFT_HEIGHT - 1);

    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);

    uint8_t *buf = (uint8_t *)frameBuffer;
    for (int i = 0; i < TFT_PIXELS * 2; i += 64) {
        hspi.writeBytes(&buf[i], 64); // Enviar en bloques de 64 bytes
    }

    digitalWrite(_cs, HIGH);
}

void ST7735::clearBuffer() {
    memset(frameBuffer, 0, sizeof(frameBuffer));
}

void ST7735::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0 || x >= TFT_WIDTH) || (y < 0 || y >= TFT_HEIGHT)) return;
    frameBuffer[y * TFT_WIDTH + x] = color;
}

void ST7735::fillScreen(uint16_t color) {
    for (int i = 0; i < TFT_PIXELS; i++) {
        frameBuffer[i] = color;
    }
}*/

void ST7735::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  while (w--) {
    drawPixel(x++, y, color);
  }
}

void ST7735::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  while (h--) {
    drawPixel(x, y++, color);
  }
}

void ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  for (int16_t dy = 0; dy < h; dy++) {
    for (int16_t dx = 0; dx < w; dx++) {
      drawPixel(x + dx, y + dy, color);
    }
  }
}

void ST7735::invertDisplay(unsigned char i) {
  writecommand(i ? 0x21 : 0x20); // INVERT ON/OFF
}

void ST7735::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg) {
  if ((x >= 128) || (y >= 160)) return;

  c -= 32;
  const uint8_t *pfont = font + (c * 5);

  for (int col = 0; col < 5; col++) {
    uint8_t bits = pgm_read_byte(pfont++);
    for (int row = 0; row < 8; row++) {
      drawPixel(x + col, y + row, (bits & 0x01) ? color : bg);
      bits >>= 1;
    }
  }
}

void ST7735::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) { swap2(x0, y0); swap2(x1, y1); }

  if (x0 > x1) {
    swap2(x0, x1);
    swap2(y0, y1);
  }

  int16_t dx = x1 - x0;
  int16_t dy = abs(y1 - y0);
  int16_t err = dx / 2;
  int16_t ystep = (y0 < y1) ? 1 : -1;

  for (; x0 <= x1; x0++) {
    if (steep) drawPixel(y0, x0, color);
    else       drawPixel(x0, y0, color);

    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void ST7735::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void ST7735::write(uint8_t c) {
  if (c == '\n') {
    cursor_y += 8;
    cursor_x = 0;
  } else if (c == '\r') {
    // Ignorar
  } else {
    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor);
    cursor_x += 6;
    if (wrap && cursor_x > (128 - 6)) {
      cursor_x = 0;
      cursor_y += 8;
    }
  }
}

void ST7735::print(const char str[]) {
  int x = 0;
  while (str[x]) {
    write(str[x]);
    x++;
  }
}

void ST7735::setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void ST7735::setTextColor(uint16_t c, uint16_t b) {
  textcolor = c;
  textbgcolor = b;
}

void ST7735::setTextWrap(uint8_t w) {
  wrap = w;
}