#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "lcd.h"

extern unsigned char _binary_fonts_dat_start[];

lcd_t *lcdInit(int i2c_dev_id, int reset_pin) {
  lcd_t *lcd;
  lcd = (lcd_t *)malloc(sizeof(lcd_t));
  lcd->buf = (unsigned char *)malloc(sizeof(char) * 384);

  pinMode(reset_pin, OUTPUT);
  lcd->fd = wiringPiI2CSetup(i2c_dev_id);

  // digitalWrite(reset_pin, HIGH);
  // delay(5);
  // digitalWrite(reset_pin, LOW);
  // delay(10);
  // digitalWrite(reset_pin, HIGH);

  unsigned char command[] = { COMMAND, DISPLAYOFF,
                              COMMAND, SETDISPLAYCLOCKDIV, 0x80,
                              COMMAND, SETMULTIPLEX, 0x2f,
                              COMMAND, SETDISPLAYOFFSET, 0x0,
                              COMMAND, SETSTARTLINE | 0x0,
                              COMMAND, CHARGEPUMP, 0x14,
                              COMMAND, NORMALDISPLAY,
                              COMMAND, DISPLAYALLONRESUME,
                              COMMAND, SEGREMAP | 0x0,
                              COMMAND, COMSCANINC,
                              COMMAND, SETCOMPINS, 0x12,
                              COMMAND, SETCONTRAST, 0x8f,
                              COMMAND, SETPRECHARGE, 0xf1,
                              COMMAND, SETVCOMDESELECT, 0x40,
                              COMMAND, DISPLAYON };

  write(lcd->fd, command, sizeof(command));

  return lcd;
}

void lcdSetPageAddress(lcd_t *lcd, unsigned char address) {
  unsigned char msg[] = { COMMAND, 0xb0 | address };
  write(lcd->fd, msg, sizeof(msg));
}

void lcdSetColumnAddress(lcd_t *lcd, unsigned char address) {
  unsigned char msg[] = { COMMAND, (0x10 | (address >> 4)) + 0x02,
                          COMMAND, 0x0f & address };
  write(lcd->fd, msg, sizeof(msg));
}

void lcdClearBuf(lcd_t *lcd) {
  for (int i = 0; i < 384; i++) {
    lcd->buf[i] = 0x00;
  }
}

void lcdCharacter(lcd_t *lcd, unsigned int row, unsigned int col, char c) {
  int fontsIndex = c - FONTS_OFFSET;

  for (int i = 0; i < 4; i++) {
    unsigned char *data = _binary_fonts_dat_start + fontsIndex * 4 + i;
    lcd->buf[row * 0x40 + col * 4 + i] = *data;
  }
}
void lcdText(lcd_t *lcd, unsigned int row, unsigned int col, const char *text) {
  for (int i = 0; i < strlen(text); i++) {
    lcdCharacter(lcd, row, col + i, text[i]);
  }
}

void lcdDrawRow(lcd_t *lcd, unsigned int row) {
  lcdSetPageAddress(lcd, row);
  lcdSetColumnAddress(lcd, 0);

  unsigned char msg[65];
  msg[0] = DATA;
  for (int i = 0; i < 64; i++) {
    msg[i + 1] = lcd->buf[row * 0x40 + i];
  }
  write(lcd->fd, msg, sizeof(msg));
}

void lcdDraw(lcd_t *lcd) {
  for (int i = 0; i < 6; i++) {
    lcdDrawRow(lcd, i);
  }
}
