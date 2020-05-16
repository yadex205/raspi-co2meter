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
  return (status & 1 << 0) > 0;
}

bool ccs811_data_available(ccs811_t *ccs811) {
  unsigned char status = ccs811_read_register(ccs811, STATUS);
  return (status & 1 << 3) > 0;
}

void ccs811_set_drive_mode(ccs811_t *ccs881, unsigned char mode) {
  if (mode > 4) {
    mode = 4;
  }

  unsigned char value;
  value = ccs811_read_register(ccs881, MEAS_MODE);
  value &= ~(0b00000111 << 4);
  value |= (mode << 4);

  unsigned char msg[] = { MEAS_MODE, value };
  write(ccs881->fd, msg, sizeof(msg));
};

ccs811_t *ccs811_init(int i2c_dev_id) {
  ccs811_t *ccs811;
  ccs811 = (ccs811_t *) malloc(sizeof(ccs811_t));
  ccs811->fd = wiringPiI2CSetup(i2c_dev_id);
  ccs811->co2 = 0;
  ccs811->tvoc = 0;

  unsigned char hw_id = ccs811_read_register(ccs811, HW_ID);
  if (hw_id != 0x81) {
    return NULL;
  }

  unsigned char msg_reset[] = { SW_RESET, 0x11, 0xe5, 0x72, 0x8a };
  write(ccs811->fd, msg_reset, sizeof(msg_reset));

  delay(1);

  if (ccs811_check_status_error(ccs811) == true) {
    return NULL;
  }
  unsigned char msg_start[] = { APP_START };
  write(ccs811->fd, msg_start, sizeof(msg_start));

  delay(1);

  ccs811_set_drive_mode(ccs811, 1);

  if (ccs811_check_status_error(ccs811) == true) {
    return NULL;
  }

  return ccs811;
}

void ccs811_read_sensor(ccs811_t *ccs811) {
  unsigned char msg[] = { ALG_RESULT_DATA };
  write(ccs811->fd, msg, sizeof(msg));

  unsigned char result[] = { 0, 0, 0, 0 };
  read(ccs811->fd, result, 4);
  ccs811->co2 = ((unsigned short) result[0] << 8) | result[1];
  ccs811->tvoc = ((unsigned short) result[2] << 8) | result[3];
}
