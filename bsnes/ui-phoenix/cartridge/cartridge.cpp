#include "../base.hpp"
Cartridge cartridge;

bool Cartridge::loadNormal(const char *basename) {
  unload();
  if(loadCartridge(SNES::memory::cartrom, baseXML, basename) == false) return false;
  SNES::cartridge.basename = baseName = nall::basename(basename);
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, { baseXML });
  loadMemory(SNES::memory::cartram, baseName, ".srm");
  loadMemory(SNES::memory::cartrtc, baseName, ".rtc");
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadBsxSlotted(const char *basename, const char *slotname) {
  unload();
  if(loadCartridge(SNES::memory::cartrom, baseXML, basename) == false) return false;
  loadCartridge(SNES::memory::bsxflash, slotAXML, slotname);
  SNES::cartridge.basename = baseName = nall::basename(basename);
  slotAName = nall::basename(slotname);
  SNES::cartridge.load(SNES::Cartridge::Mode::BsxSlotted, { baseXML, slotAXML });
  loadMemory(SNES::memory::cartram, baseName, ".srm");
  loadMemory(SNES::memory::cartrtc, baseName, ".rtc");
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadBsx(const char *basename, const char *slotname) {
  unload();
  if(loadCartridge(SNES::memory::cartrom, baseXML, basename) == false) return false;
  loadCartridge(SNES::memory::bsxflash, slotAXML, slotname);
  SNES::cartridge.basename = baseName = nall::basename(basename);
  slotAName = nall::basename(slotname);
  SNES::cartridge.load(SNES::Cartridge::Mode::Bsx, { baseXML, slotAXML });
  loadMemory(SNES::memory::bsxram, baseName, ".srm");
  loadMemory(SNES::memory::bsxpram, baseName, ".psr");
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadSufamiTurbo(const char *basename, const char *slotAname, const char *slotBname) {
  unload();
  if(loadCartridge(SNES::memory::cartrom, baseXML, basename) == false) return false;
  loadCartridge(SNES::memory::stArom, slotAXML, slotAname);
  loadCartridge(SNES::memory::stBrom, slotBXML, slotBname);
  SNES::cartridge.basename = baseName = nall::basename(basename);
  slotAName = nall::basename(slotAname);
  slotBName = nall::basename(slotBname);
  SNES::cartridge.load(SNES::Cartridge::Mode::SufamiTurbo, { baseXML, slotAXML, slotBXML });
  loadMemory(SNES::memory::stAram, slotAName, ".srm");
  loadMemory(SNES::memory::stBram, slotBName, ".srm");
  utility.cartridgeLoaded();
  return true;
}

bool Cartridge::loadSuperGameBoy(const char *basename, const char *slotname) {
  unload();
  if(loadCartridge(SNES::memory::cartrom, baseXML, basename) == false) return false;
  loadCartridge(SNES::memory::gbrom, slotAXML, slotname);
  SNES::cartridge.basename = baseName = nall::basename(basename);
  slotAName = nall::basename(slotname);
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, { baseXML, slotAXML });
  loadMemory(SNES::memory::gbram, slotAName, ".sav");
  loadMemory(SNES::memory::gbrtc, slotAName, ".rtc");
  utility.cartridgeLoaded();
  return true;
}

void Cartridge::unload() {
  patchApplied = false;
  if(SNES::cartridge.loaded() == false) return;
  saveMemory(SNES::memory::cartram, baseName, ".srm");
  saveMemory(SNES::memory::cartrtc, baseName, ".rtc");
  saveMemory(SNES::memory::bsxram, baseName, ".srm");
  saveMemory(SNES::memory::bsxpram, baseName, ".psr");
  saveMemory(SNES::memory::stAram, slotAName, ".srm");
  saveMemory(SNES::memory::stBram, slotBName, ".srm");
  saveMemory(SNES::memory::gbram, slotAName, ".sav");
  saveMemory(SNES::memory::gbrtc, slotAName, ".rtc");
  utility.cartridgeUnloaded();
  baseName = slotAName = slotBName = "";
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

bool Cartridge::loadMemory(SNES::MappedRAM &memory, string filename, const char *extension) {
  if(memory.size() == 0 || memory.size() == ~0) return true;
  filename = { filename, extension };
  if(file::exists(filename) == false) return false;
  file fp;
  if(fp.open(filename, file::mode::read)) {
    fp.read(memory.data(), min(memory.size(), fp.size()));
    fp.close();
  }
  return true;
}

bool Cartridge::saveMemory(SNES::MappedRAM &memory, string filename, const char *extension) {
  if(memory.size() == 0 || memory.size() == ~0) return true;
  filename = { filename, extension };
  file fp;
  if(fp.open(filename, file::mode::write) == false) return false;
  fp.write(memory.data(), memory.size());
  fp.close();
  return true;
}
