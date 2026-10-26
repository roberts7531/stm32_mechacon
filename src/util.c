/*
 * ps3mca-tool - PlayStation 3 Memory Card Adaptor Software
 * Copyright (C) 2011 - "someone who wants to stay anonymous"
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "util.h"
#include <stdio.h>
#if defined(WIN32) ||defined(_WIN32)
#include <windows.h>
#endif


void memrcpy(void *dst, void *src, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++) {
        ((uint8_t *)dst)[i] = ((uint8_t *)src)[len-1-i];
    }
}

void memxor(const void *a, const void *b, void *Result, size_t Length)
{
    size_t i;
    for (i = 0; i < Length; i++) {
        ((uint8_t *)Result)[i] = ((uint8_t *)a)[i] ^ ((uint8_t *)b)[i];
    }
}

void append_le_uint16(uint8_t *buf, uint16_t val)
{
    buf[0] = (uint8_t)val;
    buf[1] = (uint8_t)(val >> 8);
}

void append_le_uint32(uint8_t *buf, uint32_t val)
{
    buf[0] = (uint8_t)val;
    buf[1] = (uint8_t)(val >> 8);
    buf[2] = (uint8_t)(val >> 16);
    buf[3] = (uint8_t)(val >> 24);
}

void append_le_uint64(uint8_t *buf, uint64_t val)
{
    int i;
    for (i = 7; i >= 0; i--, val >>= 8) {
        buf[i] = (uint8_t)val;
    }
}

uint16_t read_le_uint16(const uint8_t *buf)
{
    register uint16_t val;

    val = buf[0];
    val |= (buf[1] << 8);

    return val;
}

uint32_t read_le_uint32(const uint8_t *buf)
{
    register uint32_t val;

    val = buf[0];
    val |= (buf[1] << 8);
    val |= (buf[2] << 16);
    val |= (buf[3] << 24);

    return val;
}

uint64_t read_le_uint64(const uint8_t *buf)
{
    uint64_t val = 0;
    int i;
    for (i = 0; i < 8; i++) {
        val = (val << 8) | buf[i];
    }

    return val;
}

const char* colors[COLORCNT] = {
    "\e[0;30m",
    "\e[0;34m",
    "\e[0;32m",
    "\e[0;36m",
    "\e[0;31m",
    "\e[0;35m",
    "\e[0;33m",
    "\e[0;37m",
    "\e[38;5;248m",//"\e[0;37m",
    "\e[1;34m",
    "\e[1;32m",
    "\e[1;36m",
    "\e[1;31m",
    "\e[1;35m",
    "\e[1;33m",
    "\e[1;37m",

};

char* SetConsColor(int color) {
#if defined(WIN32) || defined(_WIN32)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
    return "";
#else
    return colors[color];
#endif
}

void genericgaugepercentcalc(int curr, int max) {
    genericgaugepercent((curr*100/(float)max));
}
void genericgaugepercent(int percent) {
    genericgauge(percent*0.01);
}

void genericgauge (float progress)
{
    int barWidth = 70;

    printf("[");
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i)
    {
      if (i < pos)
        printf("=");
      else if (i == pos)
        printf(">");
      else
        printf(" ");
    }

    printf("]\r");
}
