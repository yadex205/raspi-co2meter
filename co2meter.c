/**
 * @see https://raspberry-pi.ksyic.com/page/page/pgp.id/8
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <wiringPi.h>
#include "lcd.h"
#include "ccs811.h"

#define LCD13003_ADDRESS                0x3d
#define LCD13003_PIN_RESET              12
#define CCS811_ADDRESS                  0x5b

int main(void) {
  lcd_t *lcd;
  ccs811_t *ccs811;

  wiringPiSetupGpio();
  lcd = lcdInit(LCD13003_ADDRESS, LCD13003_PIN_RESET);
  ccs811 = ccs811_init(CCS811_ADDRESS);

  lcdClearBuf(lcd);
  lcdDraw(lcd);

  int soc = socket(AF_INET, SOCK_STREAM, 0);
  struct ifreq ifr;
  ifr.ifr_addr.sa_family = AF_INET;
  strcpy(ifr.ifr_name, "eth0");
  ioctl(soc, SIOCGIFADDR, &ifr);
  close(soc);
  struct sockaddr_in addr;
  memcpy(&addr, &ifr.ifr_ifru.ifru_addr, sizeof(struct sockaddr_in));

  while(1) {
    ccs811_read_sensor(ccs811);
    lcdText(lcd, 0, 0, "IP ETH0:");
    lcdText(lcd, 1, 0, inet_ntoa(addr.sin_addr));
    if (ccs811 == NULL) {
      lcdText(lcd, 2, 0, "CO2: ERROR");
    } else {
      char result[17] = "";
      sprintf(result, "CO2: %5d", ccs811->co2);
      lcdText(lcd, 2, 0, result);
    }
    lcdDraw(lcd);
    delay(1000);
  }

  return 0;
}
