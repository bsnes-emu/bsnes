#include "../base.hpp"
Cartridge cartridge;

bool Cartridge::loadNormal(string filename) {
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

  SNES::memory::cartrom.copy(data, size);

  string baseXML = snes_information(data, size).xml_memory_map;
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, lstring() << baseXML);
  delete[] data;

  filename = string(nall::basename(filename), ".srm");
  if(SNES::memory::cartram.size() && file::exists(filename)) {
    if(fp.open(filename, file::mode_read)) {
      fp.read(SNES::memory::cartram.data(), min(fp.size(), SNES::memory::cartram.size()));
      fp.close();
    }
  }

  return true;
}
