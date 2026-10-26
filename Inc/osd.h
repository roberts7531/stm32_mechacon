#ifndef OSD_H
#define OSD_H
#include <stdint.h>

typedef enum
{
    OSD_CLOSED,
    OSD_MAIN,
    OSD_BRIGHTNESS,
    OSD_VOLUME,
    OSD_SCANLINES,
    OSD_SHUTDOWN
} OSDState;

extern OSDState osdState;
extern uint8_t osdSelection;

void osd_update();
void activate_osd();
void osd_input();
#endif