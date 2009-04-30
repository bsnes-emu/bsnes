namespace SNES {

class Gameboy {
public:
  unsigned run() {
    for(unsigned i = 0; i < memory::sgbvram.size(); i++) {
      memory::sgbvram.write(i, rand());
    }
    sgb.refresh();
    return 0;
  }

  void unload() {
  }

  void power() {
  }

  void reset() {
  }
};

};

