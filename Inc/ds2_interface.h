#ifndef DS2_INTERFACE
#include <stdint.h>
#define DS2_INTERFACE

#define CMD_POLLING                 0x42
#define CMD_CONFIG_MODE             0x43
#define CMD_PRESSURE_CONFIG         0x40
#define CMD_POLLING_CONFIG_STATUS   0x41
#define CMD_EN_ANALOG               0x44
#define CMD_IDENTITY                0x45
#define CMD_GET_CONSTANT_1          0x46
#define CMD_GET_CONSTANT_2          0x47
#define CMD_GET_CONSTANT_3          0x4C
#define CMD_EN_RUMBLE               0x4D
#define CMD_POLLING_CONFIG          0x4F


#define DEVICE_MODE_DIGITAL         0x42
#define DEVICE_MODE_ANALOG          0x73
#define DEVICE_MODE_ANALOG_PRESSURE 0x79
#define DEVICE_MODE_ESCAPE          0xF3

#define TYPE_DUALSHOCK              0x03

#define ANALOG_PRESSURE_MASK        0x03FFC000
void ds2_rst(void);
void controller_rst(void);
extern volatile uint8_t ps2_mode;
extern volatile uint8_t activeCommand;


#endif