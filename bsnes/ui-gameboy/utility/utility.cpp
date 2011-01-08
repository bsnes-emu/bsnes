#include "../base.hpp"
Utility utility;

void Utility::loadCartridge(const char *filename) {
  file fp;
  if(fp.open(filename, file::mode::read)) {
    unsigned size = fp.size();
    uint8_t *data = new uint8_t[size];
    fp.read(data, size);
    fp.close();

    cartridge.basename = nall::basename(filename);
    print(cartridge.basename, "\n");

    GameBoyCartridge info(data, size);
    GameBoy::cartridge.load(info.xml, data, size);
    delete[] data;
    GameBoy::system.power();
  }
}

bool Utility::saveState(unsigned slot) {
  GameBoy::system.runtosave();
  serializer s = GameBoy::system.serialize();

  file fp;
  if(fp.open(string(cartridge.basename, "-", slot, ".bst"), file::mode::write)) {
    fp.write(s.data(), s.size());
    fp.close();
    return true;
  }

  return false;
}

bool Utility::loadState(unsigned slot) {
  file fp;
  if(fp.open(string(cartridge.basename, "-", slot, ".bst"), file::mode::read)) {
    unsigned size = fp.size();
    uint8_t *data = new uint8_t[size];
    fp.read(data, size);
    fp.close();
    serializer s(data, size);
    return GameBoy::system.unserialize(s);
  }

  return false;
}
