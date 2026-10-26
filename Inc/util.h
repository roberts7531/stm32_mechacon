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

#ifndef __UTIL_H__
#define __UTIL_H__

#include <memory.h>
#include <stdlib.h>
#include <inttypes.h>

void memrcpy(void *dst, void *src, size_t len);

/**
 * @brief perform exclusive-or of memory buffers.
 */
void memxor(const void *a, const void *b, void *Result, size_t Length);

/**
 * @brief append an unsigned 16 bits Little Endian value to a buffer
 */
void append_le_uint16(uint8_t *buf, uint16_t val);

/**
 * @brief append an unsigned 32 bits Little Endian value to a buffer
 */
void append_le_uint32(uint8_t *buf, uint32_t val);

/**
 * @brief append an unsigned 64 bits Little Endian
 * value to a buffer
 */
void append_le_uint64(uint8_t *buf, uint64_t val);

/**
 * @brief read an unsigned 16 bits Little Endian
 * value from a buffer
 */
uint16_t read_le_uint16(const uint8_t *buf);

/**
 * @brief read an unsigned 32 bits Little Endian
 * value from a buffer
 */
uint32_t read_le_uint32(const uint8_t *buf);

/**
 * @brief read an unsigned 64 bits Little Endian
 * value from a buffer
 */
uint64_t read_le_uint64(const uint8_t *buf);

void genericgauge (float progress);


void genericgaugepercentcalc(int curr, int max);


void genericgaugepercent(int percent);

// logging and debug purposes
#define PRINTBUF(buf) {printf("%s: ", #buf); for(int x = 0; x<sizeof(buf); x++) printf("%02X ", buf[x]); printf("\n");}

char* SetConsColor(int color);
enum CONSCOLOR {
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    PURPLE,
    YELLOW,
    WHITE,
    GREY,
    SBLUE,
    SGREEN,
    SCYAN,
    SRED,
    SPURPLE,
    SYELLOW,
    SWHITE,
    COLORCNT
};
#define CCL(x) SetConsColor(x)
#define COLOR(x) puts(SetConsColor(x))
#endif

