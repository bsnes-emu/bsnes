#include <md/md.hpp>

namespace MegaDrive {

Cartridge cartridge;

auto Cartridge::load() -> bool {
  information = Information();

  if(auto pathID = interface->load(ID::MegaDrive, "Mega Drive", "md")) {
    information.pathID = pathID();
  } else return false;

  if(auto fp = interface->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  information.title = document["information/title"].text();

  return false;
}

auto Cartridge::save() -> void {
}

auto Cartridge::unload() -> void {
}

auto Cartridge::power() -> void {
}

auto Cartridge::reset() -> void {
}

}
