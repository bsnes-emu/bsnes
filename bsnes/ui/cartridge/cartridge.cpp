#include "../base.hpp"
Cartridge cartridge;

bool Cartridge::loadNormal(const char *basename) {
  unload();
  if(loadCartridge(SNES::cartridge.rom, baseXML, basename) == false) return false;
  baseName = nall::basename(basename);
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, { baseXML });
  foreach(memory, SNES::cartridge.nvram) loadMemory(memory);
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadBsxSlotted(const char *basename, const char *slotname) {
  unload();
  if(loadCartridge(SNES::cartridge.rom, baseXML, basename) == false) return false;
  loadCartridge(SNES::bsxflash.memory, bsxXML, slotname);
  baseName = nall::basename(basename);
  bsxName = nall::basename(slotname);
  SNES::cartridge.load(SNES::Cartridge::Mode::BsxSlotted, { baseXML, bsxXML });
  foreach(memory, SNES::cartridge.nvram) loadMemory(memory);
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadBsx(const char *basename, const char *slotname) {
  unload();
  if(loadCartridge(SNES::cartridge.rom, baseXML, basename) == false) return false;
  loadCartridge(SNES::bsxflash.memory, bsxXML, slotname);
  baseName = nall::basename(basename);
  bsxName = nall::basename(slotname);
  SNES::cartridge.load(SNES::Cartridge::Mode::Bsx, { baseXML, bsxXML });
  foreach(memory, SNES::cartridge.nvram) loadMemory(memory);
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadSufamiTurbo(const char *basename, const char *slotAname, const char *slotBname) {
  unload();
  if(loadCartridge(SNES::cartridge.rom, baseXML, basename) == false) return false;
  loadCartridge(SNES::sufamiturbo.slotA.rom, sufamiTurboAXML, slotAname);
  loadCartridge(SNES::sufamiturbo.slotB.rom, sufamiTurboBXML, slotBname);
  baseName = nall::basename(basename);
  sufamiTurboAName = nall::basename(slotAname);
  sufamiTurboBName = nall::basename(slotBname);
  SNES::cartridge.load(SNES::Cartridge::Mode::SufamiTurbo, { baseXML, sufamiTurboAXML, sufamiTurboBXML });
  foreach(memory, SNES::cartridge.nvram) loadMemory(memory);
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadSuperGameBoy(const char *basename, const char *slotname) {
  unload();
  if(loadCartridge(SNES::cartridge.rom, baseXML, basename) == false) return false;

  unsigned size = 0;
  uint8_t *data = 0;

  file fp;
  if(fp.open(slotname, file::mode::read)) {
    data = new uint8_t[size = fp.size()];
    fp.read(data, size);
    fp.close();
  }

  //note: it is safe to pass below two functions null pointers
  GameBoyCartridge info(data, size);
  GameBoy::cartridge.load(info.xml, data, size);
  if(data) delete[] data;

  baseName = nall::basename(basename);
  gameBoyName = nall::basename(slotname);
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, { baseXML, "" });

  foreach(memory, SNES::cartridge.nvram) loadMemory(memory);
  if(GameBoy::cartridge.info.battery && fp.open(path.load(SNES::Cartridge::Slot::GameBoy, "sav"), file::mode::read)) {
    fp.read(GameBoy::cartridge.ramdata, min(GameBoy::cartridge.ramsize, fp.size()));
    fp.close();
  }

  utility.cartridgeLoaded();
  return true;
}

void Cartridge::unload() {
  patchApplied = false;
  if(SNES::cartridge.loaded() == false) return;

  foreach(memory, SNES::cartridge.nvram) saveMemory(memory);
  if(SNES::cartridge.mode() == SNES::Cartridge::Mode::SuperGameBoy) {
    file fp;
    if(GameBoy::cartridge.info.battery && fp.open(path.load(SNES::Cartridge::Slot::GameBoy, "sav"), file::mode::write)) {
      fp.write(GameBoy::cartridge.ramdata, GameBoy::cartridge.ramsize);
      fp.close();
    }
  }

  utility.cartridgeUnloaded();
  baseName = bsxName = sufamiTurboAName = sufamiTurboBName = gameBoyName = "";
}

bool Cartridge::loadCartridge(SNES::MappedRAM &memory, string &XML, const char *filename) {
  if(file::exists(filename) == false) return false;
  file fp;
  if(fp.open(filename, file::mode::read) == false) return false;
  if(XML.readfile(string(nall::basename(filename), ".xml")) == false) XML = "";

  unsigned size = fp.size();
  uint8_t *data = new uint8_t[size];
  fp.read(data, size);
  fp.close();

  filemap patch(string(nall::basename(filename), ".ups"), filemap::mode::read);
  if(patch.opened()) {
    unsigned targetSize;
    ups patcher;
    if(patcher.apply(patch.data(), patch.size(), data, size, (uint8_t*)0, targetSize) == ups::result::target_too_small) {
      uint8_t *targetData = new uint8_t[targetSize];
      if(patcher.apply(patch.data(), patch.size(), data, size, targetData, targetSize) == ups::result::success) {
        delete[] data;
        data = targetData;
        size = targetSize;
        patchApplied = true;
      }
    }
  }

  if(XML == "") XML = SNESCartridge(data, size).xmlMemoryMap;
  memory.copy(data, size);
  delete[] data;
  return true;
}

bool Cartridge::loadMemory(SNES::Cartridge::NonVolatileRAM &memory) {
  if(memory.size == 0) return true;
  string filename = path.load(memory.slot, memory.id);
  file fp;
  if(fp.open(filename, file::mode::read) == false) return false;
  fp.read(memory.data, min(memory.size, fp.size()));
  fp.close();
  return true;
}

bool Cartridge::saveMemory(SNES::Cartridge::NonVolatileRAM &memory) {
  if(memory.size == 0) return true;
  string filename = path.load(memory.slot, memory.id);
  file fp;
  if(fp.open(filename, file::mode::write) == false) return false;
  fp.write(memory.data, memory.size);
  fp.close();
  return true;
}
