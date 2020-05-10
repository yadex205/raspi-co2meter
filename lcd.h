#ifndef LCD_H
#define LCD_H

#define PAGE               0
#define ALL                1
#define COMMAND            0x00
#define DATA               0x40
#define SETCONTRAST        0x81
#define DISPLAYALLONRESUME 0xA4
#define DISPLAYALLON       0xA5
#define NORMALDISPLAY      0xA6
#define INVERTDISPLAY      0xA7
#define DISPLAYOFF         0xAE
#define DISPLAYON          0xAF
#define SETDISPLAYOFFSET   0xD3
#define SETCOMPINS         0xDA
#define SETVCOMDESELECT    0xDB
#define SETDISPLAYCLOCKDIV 0xD5
#define SETPRECHARGE       0xD9
#define SETMULTIPLEX       0xA8
#define SETLOWCOLUMN       0x00
#define SETHIGHCOLUMN      0x10
#define SETSTARTLINE       0x40
#define MEMORYMODE         0x20
#define COMSCANINC         0xC0
#define COMSCANDEC         0xC8
#define SEGREMAP           0xA0
#define CHARGEPUMP         0x8D
#define EXTERNALVCC        0x01
#define SWITCHCAPVCC       0x02
#define FONTS_OFFSET       ' '

typedef struct _lcd_t {
  int fd;
  unsigned char *buf;
} lcd_t;

lcd_t *lcdInit(int i2c_dev_id, int reset_pin);
void lcdClearBuf(lcd_t *lcd);
void lcdText(lcd_t *lcd, unsigned int row, unsigned int col, const char *text);
void lcdDraw(lcd_t *lcd);

#endif /* !LCD_H */
