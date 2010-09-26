#include "../base.hpp"
Cartridge cartridge;

bool Cartridge::loadNormal(const char *filename) {
  unload();
  if(loadCartridge(SNES::memory::cartrom, baseXML, filename) == false) return false;
  baseName = nall::basename(filename);
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, lstring() << baseXML);
  loadMemory(SNES::memory::cartram, baseName, ".srm");
  loadMemory(SNES::memory::cartrtc, baseName, ".rtc");
  cheatEditor.load(baseName);
  utility.setTitle(notdir(baseName));
  utility.showMessage(string("Loaded ", notdir(baseName)));
  config.path.current = dir(baseName);
  return true;
}

void Cartridge::unload() {
  if(SNES::cartridge.loaded() == false) return;
  cheatEditor.save(baseName);
  saveMemory(SNES::memory::cartram, baseName, ".srm");
  saveMemory(SNES::memory::cartrtc, baseName, ".rtc");
  SNES::cartridge.unload();
}

bool Cartridge::loadCartridge(SNES::MappedRAM &memory, string &XML, const char *filename) {
  if(file::exists(filename) == false) return false;
  file fp;
  if(fp.open(filename, file::mode_read) == false) return false;

  unsigned size = fp.size();
  if((size & 0x7fff) == 512) {
    size -= 512;
    fp.seek(512);
  }
  uint8_t *data = new uint8_t[size];
  fp.read(data, size);
  fp.close();

  memory.copy(data, size);
  XML = snes_information(data, size).xml_memory_map;
  delete[] data;
  return true;
}

bool Cartridge::loadMemory(SNES::MappedRAM &memory, string filename, const char *extension) {
  if(memory.size() == 0 || memory.size() == ~0) return true;
  filename = string(filename, extension);
  if(file::exists(filename) == false) return false;
  file fp;
  if(fp.open(filename, file::mode_read)) {
    fp.read(memory.data(), min(memory.size(), fp.size()));
    fp.close();
  }
  return true;
}

bool Cartridge::saveMemory(SNES::MappedRAM &memory, string filename, const char *extension) {
  if(memory.size() == 0 || memory.size() == ~0) return true;
  filename = string(filename, extension);
  file fp;
  if(fp.open(filename, file::mode_write) == false) return false;
  fp.write(memory.data(), memory.size());
  fp.close();
  return true;
}
