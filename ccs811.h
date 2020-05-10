#ifndef CCS881_H
#define CCS881_H

#define STATUS                   0x00
#define MEAS_MODE                0x01
#define ALG_RESULT_DATA          0x02
#define RAW_DATA                 0x03
#define ENV_DATA                 0x05
#define THRESHOLDS               0x10
#define BASELINE                 0x11
#define HW_ID                    0x20
#define HW_VERSION               0x21
#define FW_BOOT_VERSION          0x23
#define FW_APP_VERSION           0x24
#define ERROR_ID                 0xE0
#define APP_START                0xF4
#define SW_RESET                 0xFF

typedef struct _ccs811_t {
  int fd;
} ccs811_t;

ccs811_t *ccs811_init(int is2_dev_id);

#endif /* !CCS881_H */
