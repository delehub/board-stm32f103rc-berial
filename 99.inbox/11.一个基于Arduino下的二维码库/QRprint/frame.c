const uint8_t neccblk1  = 1;
const uint8_t neccblk2  = 0;
const uint8_t datablkw  = 55;
const uint8_t eccblkwid = 15;
const uint8_t VERSION   = 3;
const uint8_t ECCLEVEL  = 1;
const uint8_t WD        = 29;
const uint8_t WDB       = 4;
uint8_t       strinbuf[116];
uint8_t       qrframe[600];
uint8_t       rlens[30];
#ifndef __AVR__
#define PROGMEM
#define memcpy_P memcpy
#define __LPM(x) *x
#else
#include <avr/pgmspace.h>
#endif
const uint8_t framebase[] PROGMEM = {
    0xfe, 0x00, 0x03, 0xf8,
    0x82, 0x00, 0x02, 0x08,
    0xba, 0x00, 0x02, 0xe8,
    0xba, 0x00, 0x02, 0xe8,
    0xba, 0x00, 0x02, 0xe8,
    0x82, 0x00, 0x02, 0x08,
    0xfe, 0xaa, 0xab, 0xf8,
    0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x0f, 0x80,
    0x00, 0x80, 0x08, 0x80,
    0xfe, 0x00, 0x0a, 0x80,
    0x82, 0x00, 0x08, 0x80,
    0xba, 0x00, 0x0f, 0x80,
    0xba, 0x00, 0x00, 0x00,
    0xba, 0x00, 0x00, 0x00,
    0x82, 0x00, 0x00, 0x00,
    0xfe, 0x00, 0x00, 0x00};

const uint8_t framask[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xf8, 0x10, 0x04,
    0x00, 0x80, 0x08, 0x00,
    0x40, 0x01, 0x00, 0x02,
    0x00, 0x02, 0x00, 0x01,
    0x00, 0x00, 0x40, 0x00,
    0x08, 0x00, 0x00, 0x80,
    0x03, 0xff, 0x00, 0x1f,
    0xfc, 0x00, 0x7f, 0xf8,
    0x00, 0xff, 0xf8, 0x00,
    0xff, 0xfc, 0x00, 0x01,
    0xff, 0x00, 0x00, 0x3f,
    0xe0, 0x00, 0x03, 0xfe,
    0x00, 0x00, 0x00};
