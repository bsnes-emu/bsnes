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

  if(auto pathID = interface->load(ID::SuperFamicom, "Super Famicom", "sfc")) {
    information.pathID = pathID();
  } else return false;

  if(auto fp = interface->open(ID::SuperFamicom, "manifest.bml", File::Read, File::Required)) {
    information.manifest.cartridge = fp->reads();
  } else return false;
  auto document = BML::unserialize(information.manifest.cartridge);
  loadCartridge(document);

  //Game Boy
  if(cartridge.has.ICD2) {
    information.sha256 = "";  //Game Boy cartridge not loaded yet: set later via loadGameBoy()
  }

  //BS Memory
  else if(cartridge.has.MCC && cartridge.has.BSMemorySlot) {
    information.sha256 = Hash::SHA256(bsmemory.memory.data(), bsmemory.memory.size()).digest();
  }

  //Sufami Turbo
  else if(cartridge.has.SufamiTurboSlots) {
    Hash::SHA256 sha;
    sha.data(sufamiturboA.rom.data(), sufamiturboA.rom.size());
    sha.data(sufamiturboB.rom.data(), sufamiturboB.rom.size());
    information.sha256 = sha.digest();
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
    information.sha256 = sha.digest();
  }

  rom.writeProtect(true);
  ram.writeProtect(false);
  return true;
}

auto Cartridge::loadGameBoy() -> bool {
  #if defined(SFC_SUPERGAMEBOY)
  //invoked from ICD2::load()
  information.sha256 = GameBoy::interface->sha256();
  information.manifest.gameBoy = GameBoy::interface->manifest();
  information.title.gameBoy = GameBoy::interface->title();
  loadGameBoy(BML::unserialize(information.manifest.gameBoy));
  return true;
  #endif
  return false;
}

auto Cartridge::loadBSMemory() -> bool {
  if(auto fp = interface->open(bsmemory.pathID, "manifest.bml", File::Read, File::Required)) {
    information.manifest.bsMemory = fp->reads();
  } else return false;
  loadBSMemory(BML::unserialize(information.manifest.bsMemory));
  return true;
}

auto Cartridge::loadSufamiTurboA() -> bool {
  if(auto fp = interface->open(sufamiturboA.pathID, "manifest.bml", File::Read, File::Required)) {
    information.manifest.sufamiTurboA = fp->reads();
  } else return false;
  loadSufamiTurboA(BML::unserialize(information.manifest.sufamiTurboA));
  return true;
}

auto Cartridge::loadSufamiTurboB() -> bool {
  if(auto fp = interface->open(sufamiturboB.pathID, "manifest.bml", File::Read, File::Required)) {
    information.manifest.sufamiTurboB = fp->reads();
  } else return false;
  loadSufamiTurboB(BML::unserialize(information.manifest.sufamiTurboB));
  return true;
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
