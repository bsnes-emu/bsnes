#include <ws/ws.hpp>

namespace WonderSwan {

Cartridge cartridge;
#include "memory.cpp"
#include "io.cpp"

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
    if(rom.size) rom.data = new uint8[rom.mask + 1]();
    if(rom.name) interface->loadRequest(ID::ROM, rom.name, true);
  }

  if(auto node = document["board/ram"]) {
    if(node["type"].text() == "sram") {
      ram.name = node["name"].text();
      ram.size = node["size"].natural();
      ram.mask = bit::round(ram.size) - 1;
      if(ram.size) ram.data = new uint8[ram.mask + 1]();
      if(ram.name) interface->loadRequest(ID::RAM, ram.name, false);
    }

    if(node["type"].text() == "eeprom") {
      eeprom.setName(node["name"].text());
      eeprom.setSize(node["size"].natural() / sizeof(uint16));
      eeprom.erase();
      if(eeprom.name()) interface->loadRequest(ID::EEPROM, eeprom.name(), false);
    }
  }

  information.title = document["information/title"].text();
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
}

auto Cartridge::power() -> void {
  eeprom.power();

  for(uint n = 0x00c0; n <= 0x00c8; n++) iomap[n] = this;

  r.bank_rom0 = 0xff;
  r.bank_rom1 = 0xff;
  r.bank_rom2 = 0xff;
  r.bank_sram = 0xff;
}

}
