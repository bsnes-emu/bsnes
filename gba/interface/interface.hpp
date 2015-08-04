#ifndef GBA_HPP
namespace GameBoyAdvance {
#endif

struct ID {
  enum : unsigned {
    System,
    GameBoyAdvance,
  };

  enum : unsigned {
    SystemManifest,
    BIOS,

    Manifest,
    ROM,
    RAM,
    EEPROM,
    FlashROM,
  };

  enum : unsigned {
    Device = 1,
  };
};

struct Interface : Emulator::Interface {
  string title();
  double videoFrequency();
  double audioFrequency();

  bool loaded();
  unsigned group(unsigned id);
  void load(unsigned id);
  void save();
  void load(unsigned id, const stream& stream);
  void save(unsigned id, const stream& stream);
  void unload();

  void power();
  void reset();
  void run();

  serializer serialize();
  bool unserialize(serializer&);

  void paletteUpdate(PaletteMode mode);

  Interface();

private:
  vector<Device> device;
};

extern Interface* interface;

#ifndef GBA_HPP
}
#endif
