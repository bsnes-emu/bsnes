#include <ws/ws.hpp>

namespace WonderSwan {

Cartridge cartridge;
#include "memory.cpp"
#include "rtc.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto Cartridge::Enter() -> void {
  while(true) scheduler.synchronize(), cartridge.main();
}

auto Cartridge::main() -> void {
  if(rtc.data) {
    rtcTickSecond();
    rtcCheckAlarm();
  }
  step(3'072'000);
}

auto Cartridge::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto Cartridge::power() -> void {
  create(Cartridge::Enter, 3'072'000);
  eeprom.power();

  bus.map(this, 0x00c0, 0x00c8);
  if(rtc.data) bus.map(this, 0x00ca, 0x00cb);
  bus.map(this, 0x00cc, 0x00cd);

  memory::fill(&r, sizeof(Registers));

  r.romBank0 = 0xff;
  r.romBank1 = 0xff;
  r.romBank2 = 0xff;
  r.sramBank = 0xff;
}

auto Cartridge::load() -> bool {
  information = {};

  switch(system.model()) {
  case Model::WonderSwan:
    if(auto loaded = platform->load(ID::WonderSwan, "WonderSwan", "ws")) {
      information.pathID = loaded.pathID();
    } else return false;
    break;
  case Model::WonderSwanColor:
  case Model::SwanCrystal:
    if(auto loaded = platform->load(ID::WonderSwanColor, "WonderSwan Color", "wsc")) {
      information.pathID = loaded.pathID();
    } else return false;
    break;
  }

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);

  if(auto node = document["board/rom"]) {
    rom.name = node["name"].text();
    rom.size = node["size"].natural();
    rom.mask = bit::round(rom.size) - 1;
    if(rom.size) {
      rom.data = new uint8[rom.mask + 1];
      memory::fill(rom.data, rom.mask + 1, 0xff);
      if(rom.name) if(auto fp = platform->open(pathID(), rom.name, File::Read, File::Required)) {
        fp->read(rom.data, rom.size);
      }
    }
  }

  if(auto node = document["board/ram"]) {
    if(node["type"].text() == "sram") {
      ram.name = node["name"].text();
      ram.size = node["size"].natural();
      ram.mask = bit::round(ram.size) - 1;
      if(ram.size) {
        ram.data = new uint8[ram.mask + 1];
        memory::fill(ram.data, ram.mask + 1, 0xff);
        if(ram.name) if(auto fp = platform->open(pathID(), ram.name, File::Read)) {
          fp->read(ram.data, ram.size);
        }
      }
    }

    if(node["type"].text() == "eeprom") {
      eeprom.setName(node["name"].text());
      eeprom.setSize(node["size"].natural() / sizeof(uint16));
      if(eeprom.size()) {
        eeprom.erase();
        if(eeprom.name()) if(auto fp = platform->open(pathID(), eeprom.name(), File::Read)) {
          fp->read(eeprom.data(), eeprom.size());
        }
      }
    }
  }

  if(auto node = document["board/rtc"]) {
    rtc.name = node["name"].text();
    rtc.size = node["size"].natural();
    rtc.mask = bit::round(rtc.size) - 1;
    if(rtc.size) {
      rtc.data = new uint8[rtc.mask + 1];
      memory::fill(rtc.data, rtc.mask + 1, 0x00);
      if(rtc.name) if(auto fp = platform->open(pathID(), rtc.name, File::Read)) {
        fp->read(rtc.data, rtc.size);
      }
    }
  }

  information.title = document["information/title"].text();
  information.orientation = document["information/orientation"].text() == "vertical";
  information.sha256 = Hash::SHA256(rom.data, rom.size).digest();
  return true;
}

auto Cartridge::save() -> void {
  auto document = BML::unserialize(information.manifest);

  if(auto name = document["board/ram/name"].text()) {
    if(auto fp = platform->open(pathID(), name, File::Write)) {
      fp->write(ram.data, ram.size);
    }
  }

  if(auto name = document["board/eeprom/name"].text()) {
    if(auto fp = platform->open(pathID(), name, File::Write)) {
      fp->write(eeprom.data(), eeprom.size());
    }
  }

  if(auto name = document["board/rtc/name"].text()) {
    if(auto fp = platform->open(pathID(), name, File::Write)) {
      fp->write(rtc.data, rtc.size);
    }
  }
}

auto Cartridge::unload() -> void {
  delete[] rom.data;
  rom.data = nullptr;
  rom.size = 0;
  rom.mask = 0;
  rom.name = "";

  delete[] ram.data;
  ram.data = nullptr;
  ram.size = 0;
  ram.mask = 0;
  ram.name = "";

  delete[] rtc.data;
  rtc.data = nullptr;
  rtc.size = 0;
  rtc.mask = 0;
  rtc.name = "";
}

}
