#include <ngp/ngp.hpp>

namespace NeoGeoPocket {

Cartridge cartridge;
#include "flash.cpp"
#include "serialization.cpp"

auto Cartridge::load() -> bool {
  information = {};

  if(Model::NeoGeoPocket()) {
    if(auto loaded = platform->load(ID::NeoGeoPocket, "Neo Geo Pocket", "ngp")) {
      information.pathID = loaded.pathID;
    } else return true;  //boot into BIOS
  }

  if(Model::NeoGeoPocketColor()) {
    if(auto loaded = platform->load(ID::NeoGeoPocketColor, "Neo Geo Pocket Color", "ngpc")) {
      information.pathID = loaded.pathID;
    } else return true;  //boot into BIOS
  }

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  information.title = document["game/label"].text();

  flash[0].reset(0);
  flash[1].reset(1);

  if(auto memory = document["game/board/memory(type=ROM,content=Program)"]) {
    auto size = memory["size"].natural();
    flash[0].allocate(min(16_Mibit, size));
    flash[1].allocate(size >= 16_Mibit ? 16_Mibit - size : 0);
    if(auto fp = platform->open(pathID(), "program.rom", File::Read, File::Required)) {
      flash[0].load(fp);
      flash[1].load(fp);
    } else return false;
  }

  return true;
}

auto Cartridge::save() -> void {
  auto document = BML::unserialize(information.manifest);
}

auto Cartridge::unload() -> void {
  flash[0].reset(0);
  flash[1].reset(1);
}

auto Cartridge::power() -> void {
  flash[0].power();
  flash[1].power();
}

auto Cartridge::read(uint1 chip, uint21 address) -> uint8 {
  if(!flash[0]) return 0xff;
  if(!flash[1]) chip = 0;
  return flash[chip].read(address);
}

auto Cartridge::write(uint1 chip, uint21 address, uint8 data) -> void {
  if(!flash[0]) return;
  if(!flash[1]) chip = 0;
  return flash[chip].write(address, data);
}

}
