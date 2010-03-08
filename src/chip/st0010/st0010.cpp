#include <../base.hpp>

#define ST0010_CPP
namespace SNES {

ST0010 st0010;

#include "data.hpp"
#include "opcodes.cpp"
#include "serialization.cpp"

void ST0010::init() {
}

void ST0010::enable() {
}

int16 ST0010::sin(int16 theta) {
  return sin_table[(theta >> 8) & 0xff];
}

int16 ST0010::cos(int16 theta) {
  return sin_table[((theta + 0x4000) >> 8) & 0xff];
}

uint8 ST0010::readb(uint16 addr) {
  return ram[addr & 0xfff];
}

uint16 ST0010::readw(uint16 addr) {
  return (readb(addr + 0) <<  0) |
         (readb(addr + 1) <<  8);
}

uint32 ST0010::readd(uint16 addr) {
  return (readb(addr + 0) <<  0) |
         (readb(addr + 1) <<  8) |
         (readb(addr + 2) << 16) |
         (readb(addr + 3) << 24);
}

void ST0010::writeb(uint16 addr, uint8 data) {
  ram[addr & 0xfff] = data;
}

void ST0010::writew(uint16 addr, uint16 data) {
  writeb(addr + 0, data >> 0);
  writeb(addr + 1, data >> 8);
}

void ST0010::writed(uint16 addr, uint32 data) {
  writeb(addr + 0, data >>  0);
  writeb(addr + 1, data >>  8);
  writeb(addr + 2, data >> 16);
  writeb(addr + 3, data >> 24);
}

//

void ST0010::power() {
  reset();
}

void ST0010::reset() {
  memset(ram, 0x00, sizeof ram);
}

//

uint8 ST0010::read(unsigned addr) {
  return readb(addr);
}

void ST0010::write(unsigned addr, uint8 data) {
  writeb(addr, data);

  if((addr & 0xfff) == 0x0021 && (data & 0x80)) {
    switch(ram[0x0020]) {
      case 0x01: op_01(); break;
      case 0x02: op_02(); break;
      case 0x03: op_03(); break;
      case 0x04: op_04(); break;
      case 0x05: op_05(); break;
      case 0x06: op_06(); break;
      case 0x07: op_07(); break;
      case 0x08: op_08(); break;
    }

    ram[0x0021] &= ~0x80;
  }
}

};
