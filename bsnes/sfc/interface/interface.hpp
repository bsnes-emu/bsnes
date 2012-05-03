#ifndef SFC_HPP
namespace SuperFamicom {
#endif

struct ID {
  enum : unsigned {
    IPLROM,
    ROM,
    SuperGameBoyROM,
    BsxFlashROM,
    SufamiTurboSlotAROM,
    SufamiTurboSlotBROM,
    RAM,
    RTC,
    SPC7110RTC,
    BsxRAM,
    BsxPSRAM,
  };
};

struct Interface : Emulator::Interface {
  virtual string path(unsigned slot, const string &hint) { return ""; }
  virtual void message(const string &text) {}

  bool loaded();
  void load(unsigned id, const stream &stream, const string &markup = "");
  void save(unsigned id, const stream &stream);
  void unload();

  void power();
  void reset();
  void run();

  void updatePalette();

  Interface();
};

extern Interface *interface;

#ifndef SFC_HPP
}
#endif
