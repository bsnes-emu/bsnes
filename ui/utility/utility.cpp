#include "../base.hpp"
Utility utility;

void Utility::loadCartridge(const char *filename) {
  file fp;
  if(fp.open(filename, file::mode::read)) {
    unsigned size = fp.size();
    uint8_t *data = new uint8_t[size];
    fp.read(data, size);
    fp.close();
    GameBoy::cartridge.load(data, size);
    delete[] data;
    GameBoy::system.power();
  }
}
