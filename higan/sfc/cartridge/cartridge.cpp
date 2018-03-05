#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "load.cpp"
#include "save.cpp"
#include "serialization.cpp"
Cartridge cartridge;

auto Cartridge::manifest() const -> string {
  string manifest = BML::serialize(game.document);
  manifest.append("\n", BML::serialize(board));
  if(slotGameBoy.document) manifest.append("\n", BML::serialize(slotGameBoy.document));
  if(slotBSMemory.document) manifest.append("\n", BML::serialize(slotBSMemory.document));
  if(slotSufamiTurboA.document) manifest.append("\n", BML::serialize(slotSufamiTurboA.document));
  if(slotSufamiTurboB.document) manifest.append("\n", BML::serialize(slotSufamiTurboB.document));
  return manifest;
}

auto Cartridge::title() const -> string {
  auto label = game.label;
  if(slotGameBoy.label) label.append(" + ", slotGameBoy.label);
  if(slotBSMemory.label) label.append(" + ", slotBSMemory.label);
  if(slotSufamiTurboA.label) label.append(" + ", slotSufamiTurboA.label);
  if(slotSufamiTurboB.label) label.append(" + ", slotSufamiTurboB.label);
  return label;
}

auto Cartridge::load() -> bool {
  information = {};
  has = {};
  game = {};
  slotGameBoy = {};
  slotBSMemory = {};
  slotSufamiTurboA = {};
  slotSufamiTurboB = {};

  if(auto loaded = platform->load(ID::SuperFamicom, "Super Famicom", "sfc", {"Auto", "NTSC", "PAL"})) {
    information.pathID = loaded.pathID();
    information.region = loaded.option();
  } else return false;

  if(auto fp = platform->open(ID::SuperFamicom, "manifest.bml", File::Read, File::Required)) {
    game.load(fp->reads());
  } else return false;
  loadCartridge(game.document);

  //Game Boy
  if(cartridge.has.ICD) {
    information.sha256 = "";  //Game Boy cartridge not loaded yet: set later via loadGameBoy()
  }

  //BS Memory
  else if(cartridge.has.MCC && cartridge.has.BSMemorySlot) {
    information.sha256 = Hash::SHA256(bsmemory.memory.data(), bsmemory.memory.size()).digest();
  }

  //Sufami Turbo
  else if(cartridge.has.SufamiTurboSlots) {
    Hash::SHA256 sha;
    sha.input(sufamiturboA.rom.data(), sufamiturboA.rom.size());
    sha.input(sufamiturboB.rom.data(), sufamiturboB.rom.size());
    information.sha256 = sha.digest();
  }

  //Super Famicom
  else {
    Hash::SHA256 sha;
    //hash each ROM image that exists; any with size() == 0 is ignored by sha256_chunk()
    sha.input(rom.data(), rom.size());
    sha.input(mcc.rom.data(), mcc.rom.size());
    sha.input(sa1.rom.data(), sa1.rom.size());
    sha.input(superfx.rom.data(), superfx.rom.size());
    sha.input(hitachidsp.rom.data(), hitachidsp.rom.size());
    sha.input(spc7110.prom.data(), spc7110.prom.size());
    sha.input(spc7110.drom.data(), spc7110.drom.size());
    sha.input(sdd1.rom.data(), sdd1.rom.size());
    //hash all firmware that exists
    vector<uint8> buffer;
    buffer = armdsp.firmware();
    sha.input(buffer.data(), buffer.size());
    buffer = hitachidsp.firmware();
    sha.input(buffer.data(), buffer.size());
    buffer = necdsp.firmware();
    sha.input(buffer.data(), buffer.size());
    //finalize hash
    information.sha256 = sha.digest();
  }

  rom.writeProtect(true);
  ram.writeProtect(false);
  return true;
}

auto Cartridge::loadGameBoy() -> bool {
  #if defined(SFC_SUPERGAMEBOY)
  //invoked from ICD::load()
  information.sha256 = GameBoy::cartridge.sha256();
  slotGameBoy.load(GameBoy::cartridge.manifest());
  loadGameBoy(slotGameBoy.document);
  return true;
  #endif
  return false;
}

auto Cartridge::loadBSMemory() -> bool {
  if(auto fp = platform->open(bsmemory.pathID, "manifest.bml", File::Read, File::Required)) {
    slotBSMemory.load(fp->reads());
  } else return false;
  loadBSMemory(slotBSMemory.document);
  return true;
}

auto Cartridge::loadSufamiTurboA() -> bool {
  if(auto fp = platform->open(sufamiturboA.pathID, "manifest.bml", File::Read, File::Required)) {
    slotSufamiTurboA.load(fp->reads());
  } else return false;
  loadSufamiTurboA(slotSufamiTurboA.document);
  return true;
}

auto Cartridge::loadSufamiTurboB() -> bool {
  if(auto fp = platform->open(sufamiturboB.pathID, "manifest.bml", File::Read, File::Required)) {
    slotSufamiTurboB.load(fp->reads());
  } else return false;
  loadSufamiTurboB(slotSufamiTurboB.document);
  return true;
}

auto Cartridge::save() -> void {
  saveCartridge(game.document);
  if(has.GameBoySlot) {
    saveGameBoy(slotGameBoy.document);
  }
  if(has.BSMemorySlot) {
    saveBSMemory(slotBSMemory.document);
  }
  if(has.SufamiTurboSlots) {
    saveSufamiTurboA(slotSufamiTurboA.document);
    saveSufamiTurboB(slotSufamiTurboB.document);
  }
}

auto Cartridge::unload() -> void {
  rom.reset();
  ram.reset();
}

}
