#include <gba/gba.hpp>

namespace GameBoyAdvance {

#include "mrom.cpp"
#include "sram.cpp"
#include "eeprom.cpp"
#include "flash.cpp"
#include "serialization.cpp"
Cartridge cartridge;

Cartridge::Cartridge() {
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

auto Cartridge::load() -> bool {
  information = Information();

  if(auto pathID = interface->load(ID::GameBoyAdvance, "Game Boy Advance", "gba", File::Required)) {
    information.pathID = pathID();
  } else return false;

  if(auto fp = interface->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  information.title = document["information/title"].text();

  hasSRAM   = false;
  hasEEPROM = false;
  hasFLASH  = false;

  if(auto node = document["board/rom"]) {
    mrom.size = min(32 * 1024 * 1024, node["size"].natural());
    if(auto fp = interface->open(pathID(), node["name"].text(), File::Read, File::Required)) {
      fp->read(mrom.data, mrom.size);
    }
  }

  if(auto node = document["board/ram"]) {
    if(node["type"].text() == "sram") {
      hasSRAM = true;
      sram.size = min(32 * 1024, node["size"].natural());
      sram.mask = sram.size - 1;
      for(auto n : range(sram.size)) sram.data[n] = 0xff;

      if(auto fp = interface->open(pathID(), node["name"].text(), File::Read)) {
        fp->read(sram.data, sram.size);
      }
    }

    if(node["type"].text() == "eeprom") {
      hasEEPROM = true;
      eeprom.size = min(8 * 1024, node["size"].natural());
      eeprom.bits = eeprom.size <= 512 ? 6 : 14;
      if(eeprom.size == 0) eeprom.size = 8192, eeprom.bits = 0;  //auto-detect size
      eeprom.mask = mrom.size > 16 * 1024 * 1024 ? 0x0fffff00 : 0x0f000000;
      eeprom.test = mrom.size > 16 * 1024 * 1024 ? 0x0dffff00 : 0x0d000000;
      for(auto n : range(eeprom.size)) eeprom.data[n] = 0xff;

      if(auto fp = interface->open(pathID(), node["name"].text(), File::Read)) {
        fp->read(eeprom.data, eeprom.size);
      }
    }

    if(node["type"].text() == "flash") {
      hasFLASH = true;
      flash.id = node["id"].natural();
      flash.size = min(128 * 1024, node["size"].natural());
      for(auto n : range(flash.size)) flash.data[n] = 0xff;

      //if flash ID not provided; guess that it's a Macronix chip
      //this will not work for all games; in which case, the ID must be specified manually
      if(!flash.id && flash.size ==  64 * 1024) flash.id = 0x1cc2;
      if(!flash.id && flash.size == 128 * 1024) flash.id = 0x09c2;

      if(auto fp = interface->open(pathID(), node["name"].text(), File::Read)) {
        fp->read(flash.data, flash.size);
      }
    }
  }

  information.sha256 = Hash::SHA256(mrom.data, mrom.size).digest();
  return true;
}

auto Cartridge::save() -> void {
  auto document = BML::unserialize(information.manifest);
  if(auto node = document["board/ram"]) {
    if(auto fp = interface->open(pathID(), node["name"].text(), File::Write)) {
      if(node["type"].text() == "sram") fp->write(sram.data, sram.size);
      if(node["type"].text() == "eeprom") fp->write(eeprom.data, eeprom.size);
      if(node["type"].text() == "flash") fp->write(flash.data, flash.size);
    }
  }
}

auto Cartridge::unload() -> void {
}

auto Cartridge::power() -> void {
  eeprom.power();
  flash.power();
}

#define RAM_ANALYZE

auto Cartridge::read(uint mode, uint32 addr) -> uint32 {
  if(addr < 0x0e00'0000) {
    if(hasEEPROM && (addr & eeprom.mask) == eeprom.test) return eeprom.read();
    return mrom.read(mode, addr);
  } else {
    if(hasSRAM) return sram.read(mode, addr);
    if(hasFLASH) return flash.read(addr);
    return cpu.pipeline.fetch.instruction;
  }
}

auto Cartridge::write(uint mode, uint32 addr, uint32 word) -> void {
  if(addr < 0x0e00'0000) {
    if(hasEEPROM && (addr & eeprom.mask) == eeprom.test) return eeprom.write(word & 1);
    return mrom.write(mode, addr, word);
  } else {
    if(hasSRAM) return sram.write(mode, addr, word);
    if(hasFLASH) return flash.write(addr, word);
  }
}

}
