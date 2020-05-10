/**
 * @see https://raspberry-pi.ksyic.com/page/page/pgp.id/8
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define LCD13303_PAGE                   0
#define LCD13303_ALL                    1
#define LCD13003_ADDRESS                0x3d
#define LCD13003_PIN_RESET              12
#define LCD13303_I2C_COMMAND            0x00
#define LCD13303_I2C_DATA               0x40
#define LCD13303_I2C_SETCONTRAST        0x81
#define LCD13303_I2C_DISPLAYALLONRESUME 0xA4
#define LCD13303_I2C_DISPLAYALLON       0xA5
#define LCD13303_I2C_NORMALDISPLAY      0xA6
#define LCD13303_I2C_INVERTDISPLAY      0xA7
#define LCD13303_I2C_DISPLAYOFF         0xAE
#define LCD13303_I2C_DISPLAYON          0xAF
#define LCD13303_I2C_SETDISPLAYOFFSET   0xD3
#define LCD13303_I2C_SETCOMPINS         0xDA
#define LCD13303_I2C_SETVCOMDESELECT    0xDB
#define LCD13303_I2C_SETDISPLAYCLOCKDIV 0xD5
#define LCD13303_I2C_SETPRECHARGE       0xD9
#define LCD13303_I2C_SETMULTIPLEX       0xA8
#define LCD13303_I2C_SETLOWCOLUMN       0x00
#define LCD13303_I2C_SETHIGHCOLUMN      0x10
#define LCD13303_I2C_SETSTARTLINE       0x40
#define LCD13303_I2C_MEMORYMODE         0x20
#define LCD13303_I2C_COMSCANINC         0xC0
#define LCD13303_I2C_COMSCANDEC         0xC8
#define LCD13303_I2C_SEGREMAP           0xA0
#define LCD13303_I2C_CHARGEPUMP         0x8D
#define LCD13303_I2C_EXTERNALVCC        0x01
#define LCD13303_I2C_SWITCHCAPVCC       0x02
#define FONTS_OFFSET                    ' '

const unsigned char fonts[][4] = { { 0x00, 0x00, 0x00, 0x00 }, // SPC
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0xc0, 0xc0, 0x00 }, // .
                                   { 0x00, 0x00, 0x00, 0x00 }, // /
                                   { 0x00, 0xfe, 0x82, 0xfe }, // 0
                                   { 0x00, 0x00, 0x04, 0xfe }, // 1
                                   { 0x00, 0xf2, 0x92, 0x9e }, // 2
                                   { 0x00, 0x92, 0x92, 0xfe }, // 3
                                   { 0x00, 0x1e, 0x10, 0xfe }, // 4
                                   { 0x00, 0x9e, 0x92, 0xf2 }, // 5
                                   { 0x00, 0xfe, 0x90, 0xf0 }, // 6
                                   { 0x00, 0x02, 0x02, 0xfe }, // 7
                                   { 0x00, 0xfe, 0x92, 0xfe }, // 8
                                   { 0x00, 0x1e, 0x12, 0xfe }, // 9
                                   { 0x00, 0x00, 0x44, 0x00 }, // :
                                   { 0x00, 0x00, 0xc4, 0x00 }, // ;
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0xfc, 0x0a, 0xfe }, // A
                                   { 0x00, 0xfc, 0x8a, 0xf6 }, // B
                                   { 0x00, 0xfc, 0x82, 0x82 }, // C
                                   { 0x00, 0xfe, 0x82, 0xfc }, // D
                                   { 0x00, 0xfc, 0x8a, 0x8a }, // E
                                   { 0x00, 0xfc, 0x0a, 0x0a }, // F
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x82, 0xfe, 0x82 }, // I
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0xfc, 0x0a, 0x0e }, // P
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0xfc, 0x0a, 0xf6 }, // R
                                   { 0x00, 0x86, 0x8a, 0xf2 }, // S
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
                                   { 0x00, 0x00, 0x00, 0x00 }, //
};





const unsigned char i2cMsgLcdBegin[] = { LCD13303_I2C_COMMAND, LCD13303_I2C_DISPLAYOFF,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_SETDISPLAYCLOCKDIV, 0x80,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_SETMULTIPLEX, 0x2f,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_SETDISPLAYOFFSET, 0x0,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_SETSTARTLINE | 0x0,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_CHARGEPUMP, 0x14,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_NORMALDISPLAY,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_DISPLAYALLONRESUME,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_SEGREMAP | 0x0,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_COMSCANINC,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_SETCOMPINS, 0x12,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_SETCONTRAST, 0x8f,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_SETPRECHARGE, 0xf1,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_SETVCOMDESELECT, 0x40,
                                         LCD13303_I2C_COMMAND, LCD13303_I2C_DISPLAYON };

unsigned char screenmemory [384];

void lcdSetPageAddress(int fd, unsigned char address) {
  unsigned char msg[] = { LCD13303_I2C_COMMAND, 0xb0 | address };
  write(fd, msg, sizeof(msg));
}

void lcdSetColumnAddress(int fd, unsigned char address) {
  unsigned char msg[] = { LCD13303_I2C_COMMAND, (0x10 | (address >> 4)) + 0x02,
                          LCD13303_I2C_COMMAND, 0x0f & address };
  write(fd, msg, sizeof(msg));
}

void lcdData(int fd, unsigned char data) {
  unsigned char msg[] = { LCD13303_I2C_DATA, data };
  write(fd, msg, sizeof(msg));
}

void lcdClear(int fd, unsigned char mode) {
  if (mode == LCD13303_ALL) {
    for (int i = 0; i < 8; i++) {
      lcdSetPageAddress(fd, i);
      lcdSetColumnAddress(fd, 0);

      for (int j = 0; j < 0x80; j++) {
        lcdData(fd, 0);
      }
    }
  } else {
    memset(screenmemory, 0, 384);
  }
}

void lcdDrawRow(int fd, int row) {
  lcdSetPageAddress(fd, row);
  lcdSetColumnAddress(fd, 0);

  for (int i = 0; i < 0x40; i++) {
    lcdData(fd, screenmemory[row * 0x40 + i]);
  }
}

void lcdDraw(int fd) {
  for (int i = 0; i < 6; i++) {
    lcdDrawRow(fd, i);
  }
}

void lcdPlaceCharacter(unsigned int row, unsigned int col, char c) {
  int fontsIndex = c - FONTS_OFFSET;

  for (int i = 0; i < 4; i++) {
    screenmemory[row * 0x40 + col * 4 + i] = fonts[fontsIndex][i];
  }
}

void lcdPlaceText(unsigned int row, unsigned int col, const char *text, int length) {
  for (int i = 0; i < length; i++) {
    lcdPlaceCharacter(row, col + i, text[i]);
  }
};

int main(void) {
  int fdLcd;

  wiringPiSetupGpio();
  pinMode(LCD13003_PIN_RESET, OUTPUT);
  fdLcd = wiringPiI2CSetup(LCD13003_ADDRESS);

  digitalWrite(LCD13003_PIN_RESET, HIGH);
  delay(5);
  digitalWrite(LCD13003_PIN_RESET, LOW);
  delay(10);
  digitalWrite(LCD13003_PIN_RESET, HIGH);

  write(fdLcd, i2cMsgLcdBegin, sizeof(i2cMsgLcdBegin));
  lcdClear(fdLcd, LCD13303_ALL);
  lcdClear(fdLcd, LCD13303_PAGE);

  lcdPlaceText(0, 0, "IP ADDRESS:     ", 16);
  lcdPlaceText(1, 0, "192.168.205.47  ", 16);
  lcdDraw(fdLcd);

  return 0;
}
