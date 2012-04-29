#ifndef GBA_HPP
namespace GameBoyAdvance {
#endif

struct Interface : Emulator::Interface {
  bool loaded();
  void load(unsigned id, const stream &memory, const string &markup = "");
  void unload();

  void power();
  void reset();
  void run();

  void updatePalette();

  Interface();
};

extern Interface *interface;

#ifndef GBA_HPP
}
#endif
