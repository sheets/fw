#ifndef HAL_BOARD_H
  #define HAL_BOARD_H

  #include "hal_board_cfg.h"

  #define ENABLE            1
  #define DISABLE           0

  // passkey "000000"
  #define DEV_PASSKEY       0

  // Enable or disable self test
  #define ALLOW_SELF_TEST   ENHABLE

  // Enable or DISABLE sensors
  #define ACCEL_SENSOR      DISABLE
  #define BARO_SENSOR       DISABLE
  #define GYRO_SENSOR       DISABLE
  #define HUMIDITY_SENSOR   DISABLE
  #define IRTEMP_SENSOR     ENABLE
  #define MAG_SENSOR        ENABLE

  // Config description of Tag UUID
  #define DEIVCE_NAME             "TI BLE Sensor Tag"
  #define DEV_UUID                0xe2,\
                                  0xc5,\
                                  0x6d,\
                                  0xb5,\
                                  0xdf,\
                                  0xfb,\
                                  0x48,\
                                  0xd2,\
                                  0xb0,\
                                  0x60,\
                                  0xd0,\
                                  0xf5,\
                                  0xa7,\
                                  0x10,\
                                  0x96,\
                                  0xe0
   
  // Config Device Major and Minor numbers
  #define DEV_MAJOR               0x00,\
                                  0x01
                           
  #define DEV_MINOR               0x00,\
                                  0x01

  // Config device power
  #define DEV_POWER               0xc5
                                  
#endif
