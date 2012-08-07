#include <nds/nds.hpp>

namespace NintendoDS {

Memory::~Memory() {}

struct UnmappedMemory : Memory {
  uint32 read(uint32 addr, uint32 size) { return 0u; }
  void write(uint32 addr, uint32 size, uint32 word) {}
};

static UnmappedMemory unmappedMemory;


StaticMemory::StaticMemory() { data = nullptr; size = 0u; }
StaticMemory::~StaticMemory() { if(data) delete[] data; }
uint8& StaticMemory::operator[](uint32 addr) { return data[addr]; }

uint32 StaticMemory::read(uint32 addr, uint32 size) {
  if(size == Byte) return (*this)[addr];
  if(size == Half) return addr &= ~1, (*this)[addr] | (*this)[addr+1]<<8;
  if(size == Word) return addr &= ~3, (*this)[addr] | (*this)[addr+1]<<8 | (*this)[addr+2]<<16 | (*this)[addr+3]<<24;
}

void StaticMemory::write(uint32 addr, uint32 size, uint32 word) {
  if(size == Half) addr &= ~1;
  if(size == Word) addr &= ~3;
  
  (*this)[addr] = word;
  if(size >= Half) {
    (*this)[addr+1] = word>>8;
  }
  if(size >= Word) {
    (*this)[addr+2] = word>>16;
    (*this)[addr+3] = word>>24;
  }
}


ByteMemory::ByteMemory() { data = nullptr; size = 0u; }
ByteMemory::~ByteMemory() { if(data) delete[] data; }
uint8& ByteMemory::operator[](uint32 addr) { return data[addr]; }

uint32 ByteMemory::read(uint32 addr, uint32 size) {
  return (*this)[addr] * 0x01010101;
}

void ByteMemory::write(uint32 addr, uint32 size, uint32 word) {
  (*this)[addr] = word;
}


HalfMemory::HalfMemory() { data = nullptr; size = 0u; }
HalfMemory::~HalfMemory() { if(data) delete[] data; }

uint16& HalfMemory::operator[](uint32 addr) { return data[addr>>1]; }

uint32 HalfMemory::read(uint32 addr, uint32 size) {
  if(size == Word) return addr &= ~3, (*this)[addr] + ((*this)[addr+2] << 16);
  if(size != Word) return (*this)[addr] * 0x00010001;
}

void HalfMemory::write(uint32 addr, uint32 size, uint32 word) {
  if(size == Word) addr &= ~3;
  
  (*this)[addr] = word;
  if(size == Word)
    (*this)[addr+2] = word>>16;
}


WordMemory::WordMemory() { data = nullptr; size = 0u; }
WordMemory::~WordMemory() { if(data) delete[] data; }

uint32& WordMemory::operator[](uint32 addr) { return data[addr>>2]; }

uint32 WordMemory::read(uint32 addr, uint32 size) {
  return (*this)[addr];
}

void WordMemory::write(uint32 addr, uint32 size, uint32 word) {
  if(size == Word) (*this)[addr]  = word;
  if(size != Word) (*this)[addr] ^= ((*this)[addr] ^ word) & 0xffff << 8*(addr & 2);
}


SDRAM::SDRAM() { data = nullptr; size = 0u; }
SDRAM::~SDRAM() { if(data) delete[] data; }

uint16& SDRAM::operator[](uint32 addr) { return data[addr>>1]; }

uint32 SDRAM::read(uint32 addr, uint32 size) {
  if(size == Word) return addr &= ~3, (*this)[addr] + ((*this)[addr+2] << 16);
  if(size != Word) return (*this)[addr] * 0x00010001;
}

void SDRAM::write(uint32 addr, uint32 size, uint32 word) {
  if(size == Word) {
    addr &= ~3;
    (*this)[addr+0] = word;
    (*this)[addr+2] = word>>16;
  }
  if(size == Half) (*this)[addr] = word;
  if(size == Byte) (*this)[addr] ^= ((*this)[addr] ^ word) & 0xff << 8*(addr & 1);
}


SRAM::SRAM() { data = nullptr; size = 0u; }
SRAM::~SRAM() { if(data) delete[] data; }

uint32& SRAM::operator[](uint32 addr) { return data[addr>>2]; }

uint32 SRAM::read(uint32 addr, uint32 size) {
  return (*this)[addr];
}

void SRAM::write(uint32 addr, uint32 size, uint32 word) {
  if(size == Word) (*this)[addr] = word;
  if(size == Half) (*this)[addr] ^= ((*this)[addr] ^ word) & 0xffff << 8*(addr & 2);
  if(size == Byte) (*this)[addr] ^= ((*this)[addr] ^ word) &   0xff << 8*(addr & 3);
}

#include "eeprom.cpp"
#include "fram.cpp"
#include "flash.cpp"

}
