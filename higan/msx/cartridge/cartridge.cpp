#include <msx/msx.hpp>

namespace MSX {

Cartridge cartridge;
Cartridge expansion;
#include "serialization.cpp"

auto Cartridge::load() -> bool {
  information = {};

  if(Model::MSX()) {
    if(auto loaded = platform->load(ID::MSX, "MSX", "msx", {"NTSC", "PAL"})) {
      information.pathID = loaded.pathID;
      information.region = loaded.option;
    } else return false;
  }

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  information.title = document["game/label"].text();

  if(auto memory = document["game/board/memory(type=ROM,content=Program)"]) {
    rom.allocate(memory["size"].natural());
    if(auto fp = platform->open(pathID(), "program.rom", File::Read, File::Required)) {
      rom.load(fp);
    } else return false;
  }

  if(auto memory = document["game/board/memory(type=RAM,content=Save)"]) {
    ram.allocate(memory["size"].natural());
    if(auto fp = platform->open(pathID(), "save.ram", File::Read)) {
      ram.load(fp);
    }
  }

  return true;
}

auto Cartridge::save() -> void {
  auto document = BML::unserialize(information.manifest);

  if(auto memory = document["game/board/memory(type=RAM,content=Save)"]) {
    if(auto fp = platform->open(pathID(), "save.ram", File::Write)) {
      ram.save(fp);
    }
  }
}

auto Cartridge::unload() -> void {
  rom.reset();
  ram.reset();
}

auto Cartridge::power() -> void {
}

}
