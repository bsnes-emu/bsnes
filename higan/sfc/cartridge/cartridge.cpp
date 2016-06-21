#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "load.cpp"
#include "save.cpp"
#include "serialization.cpp"
Cartridge cartridge;

auto Cartridge::manifest() const -> string {
  string manifest = information.manifest.cartridge;
  if(information.manifest.gameBoy) manifest.append("\n[[Game Boy]]\n\n", information.manifest.gameBoy);
  if(information.manifest.bsMemory) manifest.append("\n[[BS Memory]]\n\n", information.manifest.bsMemory);
  if(information.manifest.sufamiTurboA) manifest.append("\n[[Sufami Turbo - Slot A]]\n\n", information.manifest.sufamiTurboA);
  if(information.manifest.sufamiTurboB) manifest.append("\n[[Sufami Turbo - Slot B]]\n\n", information.manifest.sufamiTurboB);
  return manifest;
}

auto Cartridge::title() const -> string {
  string title = information.title.cartridge;
  if(information.title.gameBoy) title.append(" + ", information.title.gameBoy);
  if(information.title.bsMemory) title.append(" + ", information.title.bsMemory);
  if(information.title.sufamiTurboA) title.append(" + ", information.title.sufamiTurboA);
  if(information.title.sufamiTurboB) title.append(" + ", information.title.sufamiTurboB);
  return title;
}

auto Cartridge::load() -> bool {
  information = Information();
  has = Has();
  _sha256 = "";
  _region = Region::NTSC;

  if(auto fp = interface->open(ID::SuperFamicom, "manifest.bml", File::Read, File::Required)) {
    information.manifest.cartridge = fp->reads();
  } else return false;
  auto document = BML::unserialize(information.manifest.cartridge);
  loadCartridge(document);

  //Game Boy
  if(cartridge.has.ICD2) {
    _sha256 = "";  //Game Boy cartridge not loaded yet: set later via loadGameBoy()
  }

  //BS Memory
  else if(cartridge.has.MCC && cartridge.has.BSMemorySlot) {
    _sha256 = Hash::SHA256(bsmemory.memory.data(), bsmemory.memory.size()).digest();
  }

  //Sufami Turbo
  else if(cartridge.has.SufamiTurboSlots) {
    Hash::SHA256 sha;
    sha.data(sufamiturboA.rom.data(), sufamiturboA.rom.size());
    sha.data(sufamiturboB.rom.data(), sufamiturboB.rom.size());
    _sha256 = sha.digest();
  }

  //Super Famicom
  else {
    Hash::SHA256 sha;
    //hash each ROM image that exists; any with size() == 0 is ignored by sha256_chunk()
    sha.data(rom.data(), rom.size());
    sha.data(mcc.rom.data(), mcc.rom.size());
    sha.data(sa1.rom.data(), sa1.rom.size());
    sha.data(superfx.rom.data(), superfx.rom.size());
    sha.data(hitachidsp.rom.data(), hitachidsp.rom.size());
    sha.data(spc7110.prom.data(), spc7110.prom.size());
    sha.data(spc7110.drom.data(), spc7110.drom.size());
    sha.data(sdd1.rom.data(), sdd1.rom.size());
    //hash all firmware that exists
    vector<uint8> buffer;
    buffer = armdsp.firmware();
    sha.data(buffer.data(), buffer.size());
    buffer = hitachidsp.firmware();
    sha.data(buffer.data(), buffer.size());
    buffer = necdsp.firmware();
    sha.data(buffer.data(), buffer.size());
    //finalize hash
    _sha256 = sha.digest();
  }

  rom.writeProtect(true);
  ram.writeProtect(false);
  return true;
}

auto Cartridge::loadGameBoy() -> void {
  #if defined(SFC_SUPERGAMEBOY)
  //invoked from ICD2::load()
  _sha256 = GameBoy::interface->sha256();
  information.manifest.gameBoy = GameBoy::interface->manifest();
  information.title.gameBoy = GameBoy::interface->title();
  #endif
  loadGameBoy(BML::unserialize(information.manifest.gameBoy));
}

auto Cartridge::loadBSMemory() -> void {
  if(auto fp = interface->open(ID::BSMemory, "manifest.bml", File::Read, File::Required)) {
    information.manifest.bsMemory = fp->reads();
  } else return;
  loadBSMemory(BML::unserialize(information.manifest.bsMemory));
}

auto Cartridge::loadSufamiTurboA() -> void {
  if(auto fp = interface->open(ID::SufamiTurboA, "manifest.bml", File::Read, File::Required)) {
    information.manifest.sufamiTurboA = fp->reads();
  } else return;
  loadSufamiTurboA(BML::unserialize(information.manifest.sufamiTurboA));
}

auto Cartridge::loadSufamiTurboB() -> void {
  if(auto fp = interface->open(ID::SufamiTurboB, "manifest.bml", File::Read, File::Required)) {
    information.manifest.sufamiTurboB = fp->reads();
  } else return;
  loadSufamiTurboB(BML::unserialize(information.manifest.sufamiTurboB));
}

auto Cartridge::save() -> void {
  saveCartridge(BML::unserialize(information.manifest.cartridge));
  saveGameBoy(BML::unserialize(information.manifest.gameBoy));
  saveBSMemory(BML::unserialize(information.manifest.bsMemory));
  saveSufamiTurboA(BML::unserialize(information.manifest.sufamiTurboA));
  saveSufamiTurboB(BML::unserialize(information.manifest.sufamiTurboB));
}

auto Cartridge::unload() -> void {
  rom.reset();
  ram.reset();
}

}
