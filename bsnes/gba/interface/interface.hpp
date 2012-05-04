#ifndef GBA_HPP
namespace GameBoyAdvance {
#endif

struct ID {
  enum : unsigned {
    BIOS,
    ROM,
    RAM,
    EEPROM,
    FlashROM,
  };
};

struct Interface : Emulator::Interface {
  bool loaded();
  void load(unsigned id, const stream &stream, const string &markup = "");
  void save(unsigned id, const stream &stream);
  void unload();

  void power();
  void reset();
  void run();

  serializer serialize();
  bool unserialize(serializer&);

  void updatePalette();

  Interface();
};

extern Interface *interface;

#ifndef GBA_HPP
}
#endif
