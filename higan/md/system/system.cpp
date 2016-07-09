#include <md/md.hpp>

namespace MegaDrive {

System system;

auto System::run() -> void {
}

auto System::load() -> bool {
  information = Information();
  if(auto fp = interface->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;
  auto document = BML::unserialize(information.manifest);
  if(!cartridge.load()) return false;
  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  cartridge.unload();
}

auto System::power() -> void {
  cartridge.power();
  reset();
}

auto System::reset() -> void {
  cartridge.reset();
}

}
