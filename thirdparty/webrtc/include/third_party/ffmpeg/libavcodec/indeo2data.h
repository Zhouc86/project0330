/*
 * Intel Indeo 2 codec
 * copyright (c) 2005 Konstantin Shishkov
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_INDEO2DATA_H
#define AVCODEC_INDEO2DATA_H

#include <stdint.h>

#define IR2_CODES 143
static const uint16_t ir2_codes[IR2_CODES][2] = {
    { 0x0000,  3 }, { 0x0004,  3 }, { 0x0006,  3 }, { 0x0001,  5 },
    { 0x0009,  5 }, { 0x0019,  5 }, { 0x000D,  5 }, { 0x001D,  5 },
    { 0x0023,  6 }, { 0x0013,  6 }, { 0x0033,  6 }, { 0x000B,  6 },
    { 0x002B,  6 }, { 0x001B,  6 }, { 0x0007,  8 }, { 0x0087,  8 },
    { 0x0027,  8 }, { 0x00A7,  8 }, { 0x0067,  8 }, { 0x00E7,  8 },
    { 0x0097,  8 }, { 0x0057,  8 }, { 0x0037,  8 }, { 0x00B7,  8 },
    { 0x00F7,  8 }, { 0x000F,  9 }, { 0x008F,  9 }, { 0x018F,  9 },
    { 0x014F,  9 }, { 0x00CF,  9 }, { 0x002F,  9 }, { 0x012F,  9 },
    { 0x01AF,  9 }, { 0x006F,  9 }, { 0x00EF,  9 }, { 0x01EF,  9 },
    { 0x001F, 10 }, { 0x021F, 10 }, { 0x011F, 10 }, { 0x031F, 10 },
    { 0x009F, 10 }, { 0x029F, 10 }, { 0x019F, 10 }, { 0x039F, 10 },
    { 0x005F, 10 }, { 0x025F, 10 }, { 0x015F, 10 }, { 0x035F, 10 },
    { 0x00DF, 10 }, { 0x02DF, 10 }, { 0x01DF, 10 }, { 0x03DF, 10 },
    { 0x003F, 13 }, { 0x103F, 13 }, { 0x083F, 13 }, { 0x183F, 13 },
    { 0x043F, 13 }, { 0x143F, 13 }, { 0x0C3F, 13 }, { 0x1C3F, 13 },
    { 0x023F, 13 }, { 0x123F, 13 }, { 0x0A3F, 13 }, { 0x1A3F, 13 },
    { 0x063F, 13 }, { 0x163F, 13 }, { 0x0E3F, 13 }, { 0x1E3F, 13 },
    { 0x013F, 13 }, { 0x113F, 13 }, { 0x093F, 13 }, { 0x193F, 13 },
    { 0x053F, 13 }, { 0x153F, 13 }, { 0x0D3F, 13 }, { 0x1D3F, 13 },
    { 0x033F, 13 }, { 0x133F, 13 }, { 0x0B3F, 13 }, { 0x1B3F, 13 },
    { 0x073F, 13 }, { 0x173F, 13 }, { 0x0F3F, 13 }, { 0x1F3F, 13 },
    { 0x00BF, 13 }, { 0x10BF, 13 }, { 0x08BF, 13 }, { 0x18BF, 13 },
    { 0x04BF, 13 }, { 0x14BF, 13 }, { 0x0CBF, 13 }, { 0x1CBF, 13 },
    { 0x02BF, 13 }, { 0x12BF, 13 }, { 0x0ABF, 13 }, { 0x1ABF, 13 },
    { 0x06BF, 13 }, { 0x16BF, 13 }, { 0x0EBF, 13 }, { 0x1EBF, 13 },
    { 0x01BF, 13 }, { 0x11BF, 13 }, { 0x09BF, 13 }, { 0x19BF, 13 },
    { 0x05BF, 13 }, { 0x15BF, 13 }, { 0x0DBF, 13 }, { 0x1DBF, 13 },
    { 0x03BF, 13 }, { 0x13BF, 13 }, { 0x0BBF, 13 }, { 0x1BBF, 13 },
    { 0x07BF, 13 }, { 0x17BF, 13 }, { 0x0FBF, 13 }, { 0x1FBF, 13 },
    { 0x007F, 14 }, { 0x207F, 14 }, { 0x107F, 14 }, { 0x307F, 14 },
    { 0x087F, 14 }, { 0x287F, 14 }, { 0x187F, 14 }, { 0x387F, 14 },
    { 0x047F, 14 }, { 0x247F, 14 }, { 0x147F, 14 }, { 0x0002,  3 },
    { 0x0011,  5 }, { 0x0005,  5 }, { 0x0015,  5 }, { 0x0003,  6 },
    { 0x003B,  6 }, { 0x0047,  8 }, { 0x00C7,  8 }, { 0x0017,  8 },
    { 0x00D7,  8 }, { 0x0077,  8 }, { 0x010F,  9 }, { 0x004F,  9 },
    { 0x01CF,  9 }, { 0x00AF,  9 }, { 0x016F,  9 },
};

static const uint8_t ir2_delta_table[4][256] = {
    { 0x80, 0x80, 0x84, 0x84, 0x7C, 0x7C, 0x7F, 0x85,
      0x81, 0x7B, 0x85, 0x7F, 0x7B, 0x81, 0x8C, 0x8C,
      0x74, 0x74, 0x83, 0x8D, 0x7D, 0x73, 0x8D, 0x83,
      0x73, 0x7D, 0x77, 0x89, 0x89, 0x77, 0x89, 0x77,
      0x77, 0x89, 0x8C, 0x95, 0x74, 0x6B, 0x95, 0x8C,
      0x6B, 0x74, 0x7C, 0x90, 0x84, 0x70, 0x90, 0x7C,
      0x70, 0x84, 0x96, 0x96, 0x6A, 0x6A, 0x82, 0x98,
      0x7E, 0x68, 0x98, 0x82, 0x68, 0x7E, 0x97, 0xA2,
      0x69, 0x5E, 0xA2, 0x97, 0x5E, 0x69, 0xA2, 0xA2,
      0x5E, 0x5E, 0x8B, 0xA3, 0x75, 0x5D, 0xA3, 0x8B,
      0x5D, 0x75, 0x71, 0x95, 0x8F, 0x6B, 0x95, 0x71,
      0x6B, 0x8F, 0x78, 0x9D, 0x88, 0x63, 0x9D, 0x78,
      0x63, 0x88, 0x7F, 0xA7, 0x81, 0x59, 0xA7, 0x7F,
      0x59, 0x81, 0xA4, 0xB1, 0x5C, 0x4F, 0xB1, 0xA4,
      0x4F, 0x5C, 0x96, 0xB1, 0x6A, 0x4F, 0xB1, 0x96,
      0x4F, 0x6A, 0xB2, 0xB2, 0x4E, 0x4E, 0x65, 0x9B,
      0x9B, 0x65, 0x9B, 0x65, 0x65, 0x9B, 0x89, 0xB4,
      0x77, 0x4C, 0xB4, 0x89, 0x4C, 0x77, 0x6A, 0xA3,
      0x96, 0x5D, 0xA3, 0x6A, 0x5D, 0x96, 0x73, 0xAC,
      0x8D, 0x54, 0xAC, 0x73, 0x54, 0x8D, 0xB4, 0xC3,
      0x4C, 0x3D, 0xC3, 0xB4, 0x3D, 0x4C, 0xA4, 0xC3,
      0x5C, 0x3D, 0xC3, 0xA4, 0x3D, 0x5C, 0xC4, 0xC4,
      0x3C, 0x3C, 0x96, 0xC6, 0x6A, 0x3A, 0xC6, 0x96,
      0x3A, 0x6A, 0x7C, 0xBA, 0x84, 0x46, 0xBA, 0x7C,
      0x46, 0x84, 0x5B, 0xAB, 0xA5, 0x55, 0xAB, 0x5B,
      0x55, 0xA5, 0x63, 0xB4, 0x9D, 0x4C, 0xB4, 0x63,
      0x4C, 0x9D, 0x86, 0xCA, 0x7A, 0x36, 0xCA, 0x86,
      0x36, 0x7A, 0xB6, 0xD7, 0x4A, 0x29, 0xD7, 0xB6,
      0x29, 0x4A, 0xC8, 0xD7, 0x38, 0x29, 0xD7, 0xC8,
      0x29, 0x38, 0xA4, 0xD8, 0x5C, 0x28, 0xD8, 0xA4,
      0x28, 0x5C, 0x6C, 0xC1, 0x94, 0x3F, 0xC1, 0x6C,
      0x3F, 0x94, 0xD9, 0xD9, 0x27, 0x27, 0x80, 0x80, },
    { 0x80, 0x80, 0x85, 0x85, 0x7B, 0x7B, 0x7E, 0x87,
      0x82, 0x79, 0x87, 0x7E, 0x79, 0x82, 0x8F, 0x8F,
      0x71, 0x71, 0x84, 0x8F, 0x7C, 0x71, 0x8F, 0x84,
      0x71, 0x7C, 0x75, 0x8B, 0x8B, 0x75, 0x8B, 0x75,
      0x75, 0x8B, 0x8E, 0x9A, 0x72, 0x66, 0x9A, 0x8E,
      0x66, 0x72, 0x7B, 0x93, 0x85, 0x6D, 0x93, 0x7B,
      0x6D, 0x85, 0x9B, 0x9B, 0x65, 0x65, 0x82, 0x9D,
      0x7E, 0x63, 0x9D, 0x82, 0x63, 0x7E, 0x9B, 0xA8,
      0x65, 0x58, 0xA8, 0x9B, 0x58, 0x65, 0xA9, 0xA9,
      0x57, 0x57, 0x8D, 0xAA, 0x73, 0x56, 0xAA, 0x8D,
      0x56, 0x73, 0x6E, 0x99, 0x92, 0x67, 0x99, 0x6E,
      0x67, 0x92, 0x76, 0xA2, 0x8A, 0x5E, 0xA2, 0x76,
      0x5E, 0x8A, 0x7F, 0xAF, 0x81, 0x51, 0xAF, 0x7F,
      0x51, 0x81, 0xAB, 0xBA, 0x55, 0x46, 0xBA, 0xAB,
      0x46, 0x55, 0x9A, 0xBB, 0x66, 0x45, 0xBB, 0x9A,
      0x45, 0x66, 0xBB, 0xBB, 0x45, 0x45, 0x60, 0xA0,
      0xA0, 0x60, 0xA0, 0x60, 0x60, 0xA0, 0x8B, 0xBE,
      0x75, 0x42, 0xBE, 0x8B, 0x42, 0x75, 0x66, 0xAA,
      0x9A, 0x56, 0xAA, 0x66, 0x56, 0x9A, 0x70, 0xB5,
      0x90, 0x4B, 0xB5, 0x70, 0x4B, 0x90, 0xBE, 0xCF,
      0x42, 0x31, 0xCF, 0xBE, 0x31, 0x42, 0xAB, 0xD0,
      0x55, 0x30, 0xD0, 0xAB, 0x30, 0x55, 0xD1, 0xD1,
      0x2F, 0x2F, 0x9A, 0xD3, 0x66, 0x2D, 0xD3, 0x9A,
      0x2D, 0x66, 0x7B, 0xC5, 0x85, 0x3B, 0xC5, 0x7B,
      0x3B, 0x85, 0x54, 0xB4, 0xAC, 0x4C, 0xB4, 0x54,
      0x4C, 0xAC, 0x5E, 0xBE, 0xA2, 0x42, 0xBE, 0x5E,
      0x42, 0xA2, 0x87, 0xD8, 0x79, 0x28, 0xD8, 0x87,
      0x28, 0x79, 0xC0, 0xE8, 0x40, 0x18, 0xE8, 0xC0,
      0x18, 0x40, 0xD5, 0xE8, 0x2B, 0x18, 0xE8, 0xD5,
      0x18, 0x2B, 0xAB, 0xE9, 0x55, 0x17, 0xE9, 0xAB,
      0x17, 0x55, 0x68, 0xCD, 0x98, 0x33, 0xCD, 0x68,
      0x33, 0x98, 0xEA, 0xEA, 0x16, 0x16, 0x80, 0x80, },
    { 0x80, 0x80, 0x86, 0x86, 0x7A, 0x7A, 0x7E, 0x88,
      0x82, 0x78, 0x88, 0x7E, 0x78, 0x82, 0x92, 0x92,
      0x6E, 0x6E, 0x85, 0x92, 0x7B, 0x6E, 0x92, 0x85,
      0x6E, 0x7B, 0x73, 0x8D, 0x8D, 0x73, 0x8D, 0x73,
      0x73, 0x8D, 0x91, 0x9E, 0x6F, 0x62, 0x9E, 0x91,
      0x62, 0x6F, 0x79, 0x97, 0x87, 0x69, 0x97, 0x79,
      0x69, 0x87, 0xA0, 0xA0, 0x60, 0x60, 0x83, 0xA2,
      0x7D, 0x5E, 0xA2, 0x83, 0x5E, 0x7D, 0xA0, 0xB0,
      0x60, 0x50, 0xB0, 0xA0, 0x50, 0x60, 0xB1, 0xB1,
      0x4F, 0x4F, 0x8F, 0xB2, 0x71, 0x4E, 0xB2, 0x8F,
      0x4E, 0x71, 0x6B, 0x9E, 0x95, 0x62, 0x9E, 0x6B,
      0x62, 0x95, 0x74, 0xA9, 0x8C, 0x57, 0xA9, 0x74,
      0x57, 0x8C, 0x7F, 0xB8, 0x81, 0x48, 0xB8, 0x7F,
      0x48, 0x81, 0xB4, 0xC5, 0x4C, 0x3B, 0xC5, 0xB4,
      0x3B, 0x4C, 0x9F, 0xC6, 0x61, 0x3A, 0xC6, 0x9F,
      0x3A, 0x61, 0xC6, 0xC6, 0x3A, 0x3A, 0x59, 0xA7,
      0xA7, 0x59, 0xA7, 0x59, 0x59, 0xA7, 0x8D, 0xCA,
      0x73, 0x36, 0xCA, 0x8D, 0x36, 0x73, 0x61, 0xB2,
      0x9F, 0x4E, 0xB2, 0x61, 0x4E, 0x9F, 0x6D, 0xBF,
      0x93, 0x41, 0xBF, 0x6D, 0x41, 0x93, 0xCA, 0xDF,
      0x36, 0x21, 0xDF, 0xCA, 0x21, 0x36, 0xB3, 0xDF,
      0x4D, 0x21, 0xDF, 0xB3, 0x21, 0x4D, 0xE1, 0xE1,
      0x1F, 0x1F, 0x9F, 0xE3, 0x61, 0x1D, 0xE3, 0x9F,
      0x1D, 0x61, 0x7A, 0xD3, 0x86, 0x2D, 0xD3, 0x7A,
      0x2D, 0x86, 0x4C, 0xBE, 0xB4, 0x42, 0xBE, 0x4C,
      0x42, 0xB4, 0x57, 0xCA, 0xA9, 0x36, 0xCA, 0x57,
      0x36, 0xA9, 0x88, 0xE9, 0x78, 0x17, 0xE9, 0x88,
      0x17, 0x78, 0xCC, 0xFB, 0x34, 0x05, 0xFB, 0xCC,
      0x05, 0x34, 0xE6, 0xFB, 0x1A, 0x05, 0xFB, 0xE6,
      0x05, 0x1A, 0xB4, 0xFD, 0x4C, 0x03, 0xFD, 0xB4,
      0x03, 0x4C, 0x63, 0xDC, 0x9D, 0x24, 0xDC, 0x63,
      0x24, 0x9D, 0xFE, 0xFE, 0x02, 0x02, 0x80, 0x80, },
    { 0x80, 0x80, 0x87, 0x87, 0x79, 0x79, 0x7E, 0x89,
      0x82, 0x77, 0x89, 0x7E, 0x77, 0x82, 0x95, 0x95,
      0x6B, 0x6B, 0x86, 0x96, 0x7A, 0x6A, 0x96, 0x86,
      0x6A, 0x7A, 0x70, 0x90, 0x90, 0x70, 0x90, 0x70,
      0x70, 0x90, 0x94, 0xA4, 0x6C, 0x5C, 0xA4, 0x94,
      0x5C, 0x6C, 0x78, 0x9B, 0x88, 0x65, 0x9B, 0x78,
      0x65, 0x88, 0xA6, 0xA6, 0x5A, 0x5A, 0x83, 0xA9,
      0x7D, 0x57, 0xA9, 0x83, 0x57, 0x7D, 0xA6, 0xB9,
      0x5A, 0x47, 0xB9, 0xA6, 0x47, 0x5A, 0xBA, 0xBA,
      0x46, 0x46, 0x92, 0xBC, 0x6E, 0x44, 0xBC, 0x92,
      0x44, 0x6E, 0x67, 0xA3, 0x99, 0x5D, 0xA3, 0x67,
      0x5D, 0x99, 0x72, 0xB0, 0x8E, 0x50, 0xB0, 0x72,
      0x50, 0x8E, 0x7F, 0xC3, 0x81, 0x3D, 0xC3, 0x7F,
      0x3D, 0x81, 0xBE, 0xD2, 0x42, 0x2E, 0xD2, 0xBE,
      0x2E, 0x42, 0xA5, 0xD4, 0x5B, 0x2C, 0xD4, 0xA5,
      0x2C, 0x5B, 0xD4, 0xD4, 0x2C, 0x2C, 0x52, 0xAE,
      0xAE, 0x52, 0xAE, 0x52, 0x52, 0xAE, 0x8F, 0xD8,
      0x71, 0x28, 0xD8, 0x8F, 0x28, 0x71, 0x5B, 0xBB,
      0xA5, 0x45, 0xBB, 0x5B, 0x45, 0xA5, 0x69, 0xCB,
      0x97, 0x35, 0xCB, 0x69, 0x35, 0x97, 0xD8, 0xF0,
      0x28, 0x10, 0xF0, 0xD8, 0x10, 0x28, 0xBD, 0xF1,
      0x43, 0x0F, 0xF1, 0xBD, 0x0F, 0x43, 0xF3, 0xF3,
      0x0D, 0x0D, 0xA5, 0xF6, 0x5B, 0x0A, 0xF6, 0xA5,
      0x0A, 0x5B, 0x78, 0xE2, 0x88, 0x1E, 0xE2, 0x78,
      0x1E, 0x88, 0x42, 0xC9, 0xBE, 0x37, 0xC9, 0x42,
      0x37, 0xBE, 0x4F, 0xD8, 0xB1, 0x28, 0xD8, 0x4F,
      0x28, 0xB1, 0x8A, 0xFD, 0x76, 0x03, 0xFD, 0x8A,
      0x03, 0x76, 0xDB, 0xFF, 0x25, 0x01, 0xFF, 0xDB,
      0x01, 0x25, 0xF9, 0xFF, 0x07, 0x01, 0xFF, 0xF9,
      0x01, 0x07, 0xBE, 0xFF, 0x42, 0x01, 0xFF, 0xBE,
      0x01, 0x42, 0x5E, 0xED, 0xA2, 0x13, 0xED, 0x5E,
      0x13, 0xA2, 0xFF, 0xFF, 0x01, 0x01, 0x80, 0x80, },
};

#endif /* AVCODEC_INDEO2DATA_H */
