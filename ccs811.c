#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "ccs811.h"

unsigned char ccs811_read_register(ccs811_t *ccs811, unsigned char register_id) {
  unsigned char value = 0;

  unsigned char msg[] = { register_id };
  write(ccs811->fd, msg, sizeof(msg));
  read(ccs811->fd, &value, 1);

  return value;
}

int ccs811_check_status_error(ccs811_t *ccs811) {
  unsigned char status = ccs811_read_register(ccs811, STATUS);
  return status & 1 << 0;
}

ccs811_t *ccs811_init(int i2c_dev_id) {
  ccs811_t *ccs811;
  ccs811 = (ccs811_t *) malloc(sizeof(ccs811_t));
  ccs811->fd = wiringPiI2CSetup(i2c_dev_id);

  unsigned char hw_id = ccs811_read_register(ccs811, HW_ID);
  if (hw_id != 0x81) {
    return NULL;
  }

  unsigned char msg_reset[] = { SW_RESET, 0x11, 0xe5, 0x72, 0x8a };
  write(ccs811->fd, msg_reset, sizeof(msg_reset));

  delay(10);

  if (ccs811_check_status_error(ccs811) == true) {
    return NULL;
  }

  return ccs811;
}
