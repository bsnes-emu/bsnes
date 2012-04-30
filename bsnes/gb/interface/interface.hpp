#ifndef GB_HPP
namespace GameBoy {
#endif

struct Interface : Emulator::Interface {
  //Super Game Boy bindings
  virtual void lcdScanline() {}
  virtual void joypWrite(bool p15, bool p14) {}

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

#ifndef GB_HPP
}
#endif
