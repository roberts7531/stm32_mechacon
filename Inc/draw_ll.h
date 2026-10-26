#ifndef DRAW_LL_H
#define DRAW_LL_H
#include <stdbool.h>
void osd_set_pixel(int x, int y, bool on);
void osd_clear(bool on);
void osd_draw_rect(int x, int y, int w, int h, bool on);
void osd_fill_rect(int x, int y, int w, int h, int fill,
                   bool vertical, bool segmented);
void osd_upload(void);
void osd_draw_string(int x, int y, const char *str,bool inverted);
#endif