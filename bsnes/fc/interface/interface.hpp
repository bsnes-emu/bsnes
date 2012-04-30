#ifndef FC_HPP
namespace Famicom {
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

#ifndef FC_HPP
}
#endif
