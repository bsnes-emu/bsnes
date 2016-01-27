#include <ws/ws.hpp>

namespace WonderSwan {

Cartridge cartridge;
#include "memory.cpp"

auto Cartridge::loaded() const -> bool {
  return _loaded;
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
    if(rom.size) rom.data = new uint8[rom.size]();
    if(rom.name) interface->loadRequest(ID::ROM, rom.name, true);
  }

  if(auto node = document["board/ram"]) {
    ram.name = node["name"].text();
    ram.size = node["size"].natural();
    if(ram.size) ram.data = new uint8[ram.size]();
    if(ram.name) interface->loadRequest(ID::RAM, ram.name, true);
  }

  information.title = document["information/title"].text();
  information.sha256 = Hash::SHA256(rom.data, rom.size).digest();
  _loaded = true;
}

auto Cartridge::unload() -> void {
  _loaded = false;

  delete[] rom.data;
  rom.data = nullptr;
  rom.size = 0;
  rom.name = "";

  delete[] ram.data;
  ram.data = nullptr;
  ram.size = 0;
  ram.name = "";
}

auto Cartridge::power() -> void {
  r.bank_rom0 = 0x00;
  r.bank_rom1 = 0x00;
  r.bank_rom2 = 0x00;
  r.bank_sram = 0x00;
}

}
