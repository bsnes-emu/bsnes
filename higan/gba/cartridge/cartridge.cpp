#include <gba/gba.hpp>

namespace GameBoyAdvance {

Cartridge cartridge;
#include "mrom.cpp"
#include "sram.cpp"
#include "eeprom.cpp"
#include "flash.cpp"
#include "serialization.cpp"

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

  if(auto loaded = platform->load(ID::GameBoyAdvance, "Game Boy Advance", "gba")) {
    information.pathID = loaded.pathID;
  } else return false;

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  information.title = document["game/label"].text();

  hasSRAM   = false;
  hasEEPROM = false;
  hasFLASH  = false;

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=ROM,content=Program)"]}) {
    mrom.size = min(32 * 1024 * 1024, (uint)memory.size);
    if(auto fp = platform->open(pathID(), memory.name(), File::Read, File::Required)) {
      fp->read(mrom.data, mrom.size);
    }
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=RAM,content=Save)"]}) {
    hasSRAM = true;
    sram.size = min(32 * 1024, (uint)memory.size);
    sram.mask = sram.size - 1;
    for(auto n : range(sram.size)) sram.data[n] = 0xff;

    if(memory.nonVolatile) {
      if(auto fp = platform->open(pathID(), memory.name(), File::Read)) {
        fp->read(sram.data, sram.size);
      }
    }
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=EEPROM,content=Save)"]}) {
    hasEEPROM = true;
    eeprom.size = min(8 * 1024, (uint)memory.size);
    eeprom.bits = eeprom.size <= 512 ? 6 : 14;
    if(eeprom.size == 0) eeprom.size = 8192, eeprom.bits = 0;  //auto-detect size
    eeprom.mask = mrom.size > 16 * 1024 * 1024 ? 0x0fffff00 : 0x0f000000;
    eeprom.test = mrom.size > 16 * 1024 * 1024 ? 0x0dffff00 : 0x0d000000;
    for(auto n : range(eeprom.size)) eeprom.data[n] = 0xff;

    if(auto fp = platform->open(pathID(), memory.name(), File::Read)) {
      fp->read(eeprom.data, eeprom.size);
    }
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=Flash,content=Save)"]}) {
    hasFLASH = true;
    flash.size = min(128 * 1024, (uint)memory.size);
    flash.manufacturer = memory.manufacturer;
    for(auto n : range(flash.size)) flash.data[n] = 0xff;

    flash.id = 0;
    if(flash.manufacturer == "Atmel"     && flash.size ==  64 * 1024) flash.id = 0x3d1f;
    if(flash.manufacturer == "Macronix"  && flash.size ==  64 * 1024) flash.id = 0x1cc2;
    if(flash.manufacturer == "Macronix"  && flash.size == 128 * 1024) flash.id = 0x09c2;
    if(flash.manufacturer == "Panasonic" && flash.size ==  64 * 1024) flash.id = 0x1b32;
    if(flash.manufacturer == "Sanyo"     && flash.size == 128 * 1024) flash.id = 0x1362;
    if(flash.manufacturer == "SST"       && flash.size ==  64 * 1024) flash.id = 0xd4bf;

    if(auto fp = platform->open(pathID(), memory.name(), File::Read)) {
      fp->read(flash.data, flash.size);
    }
  }

  information.sha256 = Hash::SHA256(mrom.data, mrom.size).digest();
  return true;
}

auto Cartridge::save() -> void {
  auto document = BML::unserialize(information.manifest);

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=RAM,content=Save)"]}) {
    if(memory.nonVolatile) {
      if(auto fp = platform->open(pathID(), memory.name(), File::Write)) {
        fp->write(sram.data, sram.size);
      }
    }
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=EEPROM,content=Save)"]}) {
    if(auto fp = platform->open(pathID(), memory.name(), File::Write)) {
      fp->write(eeprom.data, eeprom.size);
    }
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=Flash,content=Save)"]}) {
    if(auto fp = platform->open(pathID(), memory.name(), File::Write)) {
      fp->write(flash.data, flash.size);
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
