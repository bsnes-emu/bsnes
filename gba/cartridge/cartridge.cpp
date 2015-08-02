#include <gba/gba.hpp>

namespace GameBoyAdvance {

#include "eeprom.cpp"
#include "flashrom.cpp"
#include "serialization.cpp"
Cartridge cartridge;

string Cartridge::title() {
  return information.title;
}

void Cartridge::load() {
  interface->loadRequest(ID::Manifest, "manifest.bml");

  auto document = BML::unserialize(information.markup);
  information.title = document["information/title"].text();

  unsigned rom_size = 0;
  if(document["cartridge/rom"]) {
    auto info = document["cartridge/rom"];
    interface->loadRequest(ID::ROM, info["name"].text());
    rom_size = info["size"].decimal();
    for(unsigned addr = rom_size; addr < rom.size; addr++) {
      rom.data[addr] = rom.data[Bus::mirror(addr, rom_size)];
    }
  }

  has_sram     = false;
  has_eeprom   = false;
  has_flashrom = false;

  if(document["cartridge/ram"]) {
    auto info = document["cartridge/ram"];

    if(info["type"].text() == "SRAM" || info["type"].text() == "FRAM") {
      has_sram = true;
      ram.size = info["size"].decimal();
      ram.mask = ram.size - 1;
      for(unsigned n = 0; n < ram.size; n++) ram.data[n] = 0xff;

      interface->loadRequest(ID::RAM, info["name"].text());
      memory.append({ID::RAM, info["name"].text()});
    }

    if(info["type"].text() == "EEPROM") {
      has_eeprom = true;
      eeprom.size = info["size"].decimal();
      eeprom.bits = eeprom.size <= 512 ? 6 : 14;
      if(eeprom.size == 0) eeprom.size = 8192, eeprom.bits = 0;  //auto-detect size
      eeprom.mask = rom_size > 16 * 1024 * 1024 ? 0x0fffff00 : 0x0f000000;
      eeprom.test = rom_size > 16 * 1024 * 1024 ? 0x0dffff00 : 0x0d000000;
      for(unsigned n = 0; n < eeprom.size; n++) eeprom.data[n] = 0xff;

      interface->loadRequest(ID::EEPROM, info["name"].text());
      memory.append({ID::EEPROM, info["name"].text()});
    }

    if(info["type"].text() == "FlashROM") {
      has_flashrom = true;
      flashrom.id = info["id"].decimal();
      flashrom.size = info["size"].decimal();
      for(unsigned n = 0; n < flashrom.size; n++) flashrom.data[n] = 0xff;

      //if FlashROM ID not provided; guess that it's a Macronix chip
      //this will not work for all games; in which case, the ID must be specified manually
      if(!flashrom.id && flashrom.size ==  64 * 1024) flashrom.id = 0x1cc2;
      if(!flashrom.id && flashrom.size == 128 * 1024) flashrom.id = 0x09c2;

      interface->loadRequest(ID::FlashROM, info["name"].text());
      memory.append({ID::FlashROM, info["name"].text()});
    }
  }

  sha256 = Hash::SHA256(rom.data, rom_size).digest();

  system.load();
  loaded = true;
}

void Cartridge::unload() {
  if(loaded == false) return;
  loaded = false;
  memory.reset();
}

void Cartridge::power() {
  eeprom.power();
  flashrom.power();
}

uint8* Cartridge::ram_data() {
  if(has_sram) return ram.data;
  if(has_eeprom) return eeprom.data;
  if(has_flashrom) return flashrom.data;
  return nullptr;
}

unsigned Cartridge::ram_size() {
  if(has_sram) return ram.size;
  if(has_eeprom) return eeprom.size;
  if(has_flashrom) return flashrom.size;
  return 0;
}

auto Cartridge::read(uint8 *data, unsigned mode, uint32 addr) -> uint32 {
  if(mode & Word) addr &= ~3;
  if(mode & Half) addr &= ~1;
  data += addr;
  if(mode & Word) return data[0] << 0 | data[1] << 8 | data[2] << 16 | data[3] << 24;
  if(mode & Half) return data[0] << 0 | data[1] << 8;
  if(mode & Byte) return data[0];
  return 0;  //should never occur
}

auto Cartridge::write(uint8 *data, unsigned mode, uint32 addr, uint32 word) -> void {
  if(mode & Word) addr &= ~3;
  if(mode & Half) addr &= ~1;
  data += addr;
  if(mode & Word) {
    data[0] = word >>  0;
    data[1] = word >>  8;
    data[2] = word >> 16;
    data[3] = word >> 24;
  } else if(mode & Half) {
    data[0] = word >>  0;
    data[1] = word >>  8;
  } else if(mode & Byte) {
    data[0] = word >>  0;
  }
}

#define RAM_ANALYZE

auto Cartridge::rom_read(unsigned mode, uint32 addr) -> uint32 {
  if(has_eeprom && (addr & eeprom.mask) == eeprom.test) return eeprom.read();
  return read(rom.data, mode, addr & 0x01ff'ffff);
}

auto Cartridge::rom_write(unsigned mode, uint32 addr, uint32 word) -> void {
  if(has_eeprom && (addr & eeprom.mask) == eeprom.test) return eeprom.write(word & 1);
}

auto Cartridge::ram_read(unsigned mode, uint32 addr) -> uint32 {
  if(has_sram) return read(ram.data, mode, addr & ram.mask);
  if(has_flashrom) return flashrom.read(addr);
  return cpu.pipeline.fetch.instruction;
}

auto Cartridge::ram_write(unsigned mode, uint32 addr, uint32 word) -> void {
  if(has_sram) return write(ram.data, mode, addr & ram.mask, word);
  if(has_flashrom) return flashrom.write(addr, word);
}

Cartridge::Cartridge() {
  loaded = false;
  rom.data = new uint8[rom.size = 32 * 1024 * 1024];
  ram.data = new uint8[ram.size = 32 * 1024];
  eeprom.data = new uint8[eeprom.size = 8 * 1024];
  flashrom.data = new uint8[flashrom.size = 128 * 1024];
}

Cartridge::~Cartridge() {
  delete[] rom.data;
  delete[] ram.data;
  delete[] eeprom.data;
  delete[] flashrom.data;
}

}
