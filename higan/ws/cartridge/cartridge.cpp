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
  clock += clocks;
  if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
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

auto Cartridge::load() -> void {
  information.manifest = "";
  information.title = "";
  information.sha256 = "";

  interface->loadRequest(ID::Manifest, "manifest.bml", true);
  auto document = BML::unserialize(information.manifest);

  if(auto node = document["board/rom"]) {
    rom.name = node["name"].text();
    rom.size = node["size"].natural();
    rom.mask = bit::round(rom.size) - 1;
    if(rom.size) {
      rom.data = new uint8[rom.mask + 1];
      memory::fill(rom.data, rom.mask + 1, 0xff);
      if(rom.name) interface->loadRequest(ID::ROM, rom.name, true);
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
        if(ram.name) interface->loadRequest(ID::RAM, ram.name, false);
      }
    }

    if(node["type"].text() == "eeprom") {
      eeprom.setName(node["name"].text());
      eeprom.setSize(node["size"].natural() / sizeof(uint16));
      if(eeprom.size()) {
        eeprom.erase();
        if(eeprom.name()) interface->loadRequest(ID::EEPROM, eeprom.name(), false);
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
      if(rtc.name) interface->loadRequest(ID::RTC, rtc.name, false);
    }
  }

  information.title = document["information/title"].text();
  information.orientation = document["information/orientation"].text() == "vertical";
  information.sha256 = Hash::SHA256(rom.data, rom.size).digest();
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
