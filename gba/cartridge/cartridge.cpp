#include <gba/gba.hpp>

namespace GameBoyAdvance {

#include "mrom.cpp"
#include "sram.cpp"
#include "eeprom.cpp"
#include "flash.cpp"
#include "serialization.cpp"
Cartridge cartridge;

Cartridge::Cartridge() {
  loaded = false;
  mrom.data = new uint8[mrom.size = 32 * 1024 * 1024];
  sram.data = new uint8[sram.size = 32 * 1024];
  eeprom.data = new uint8[eeprom.size = 8 * 1024];
  flash.data = new uint8[flash.size = 128 * 1024];
}

Cartridge::~Cartridge() {
  delete[] mrom.data;
  delete[] sram.data;
  delete[] eeprom.data;
  delete[] flash.data;
}

auto Cartridge::title() const -> string {
  return information.title;
}

auto Cartridge::load() -> void {
  interface->loadRequest(ID::Manifest, "manifest.bml", true);

  auto document = BML::unserialize(information.markup);
  information.title = document["information/title"].text();

  unsigned mrom_size = 0;
  if(auto info = document["cartridge/mrom"]) {
    interface->loadRequest(ID::MROM, info["name"].text(), true);
    mrom_size = info["size"].decimal();
    for(unsigned addr = mrom_size; addr < mrom.size; addr++) {
      mrom.data[addr] = mrom.data[Bus::mirror(addr, mrom_size)];
    }
  }

  has_sram   = false;
  has_eeprom = false;
  has_flash  = false;

  if(auto info = document["cartridge/sram"]) {
    has_sram = true;
    sram.size = min(32 * 1024, info["size"].decimal());
    sram.mask = sram.size - 1;
    for(auto n : range(sram.size)) sram.data[n] = 0xff;

    interface->loadRequest(ID::SRAM, info["name"].text(), false);
    memory.append({ID::SRAM, info["name"].text()});
  }

  if(auto info = document["cartridge/eeprom"]) {
    has_eeprom = true;
    eeprom.size = min(8 * 1024, info["size"].decimal());
    eeprom.bits = eeprom.size <= 512 ? 6 : 14;
    if(eeprom.size == 0) eeprom.size = 8192, eeprom.bits = 0;  //auto-detect size
    eeprom.mask = mrom_size > 16 * 1024 * 1024 ? 0x0fffff00 : 0x0f000000;
    eeprom.test = mrom_size > 16 * 1024 * 1024 ? 0x0dffff00 : 0x0d000000;
    for(auto n : range(eeprom.size)) eeprom.data[n] = 0xff;

    interface->loadRequest(ID::EEPROM, info["name"].text(), false);
    memory.append({ID::EEPROM, info["name"].text()});
  }

  if(auto info = document["cartridge/flash"]) {
    has_flash = true;
    flash.id = info["id"].decimal();
    flash.size = min(128 * 1024, info["size"].decimal());
    for(auto n : range(flash.size)) flash.data[n] = 0xff;

    //if flash ID not provided; guess that it's a Macronix chip
    //this will not work for all games; in which case, the ID must be specified manually
    if(!flash.id && flash.size ==  64 * 1024) flash.id = 0x1cc2;
    if(!flash.id && flash.size == 128 * 1024) flash.id = 0x09c2;

    interface->loadRequest(ID::FLASH, info["name"].text(), false);
    memory.append({ID::FLASH, info["name"].text()});
  }

  sha256 = Hash::SHA256(mrom.data, mrom_size).digest();

  system.load();
  loaded = true;
}

auto Cartridge::unload() -> void {
  if(loaded) {
    loaded = false;
    memory.reset();
  }
}

auto Cartridge::power() -> void {
  eeprom.power();
  flash.power();
}

#define RAM_ANALYZE

auto Cartridge::read(unsigned mode, uint32 addr) -> uint32 {
  if(addr < 0x0e00'0000) {
    if(has_eeprom && (addr & eeprom.mask) == eeprom.test) return eeprom.read();
    return mrom.read(mode, addr);
  } else {
    if(has_sram) return sram.read(mode, addr);
    if(has_flash) return flash.read(addr);
    return cpu.pipeline.fetch.instruction;
  }
}

auto Cartridge::write(unsigned mode, uint32 addr, uint32 word) -> void {
  if(addr < 0x0e00'0000) {
    if(has_eeprom && (addr & eeprom.mask) == eeprom.test) return eeprom.write(word & 1);
    return mrom.write(mode, addr, word);
  } else {
    if(has_sram) return sram.write(mode, addr, word);
    if(has_flash) return flash.write(addr, word);
  }
}

}
