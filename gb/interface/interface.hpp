#ifndef GB_HPP
namespace GameBoy {
#endif

struct ID {
  enum : unsigned {
    System,
    GameBoy,
    SuperGameBoy,
    GameBoyColor,
  };

  enum : unsigned {
    SystemManifest,
    GameBoyBootROM,
    SuperGameBoyBootROM,
    GameBoyColorBootROM,

    Manifest,
    ROM,
    RAM,
  };

  enum : unsigned {
    Device = 1,
  };
};

struct Interface : Emulator::Interface {
  //Super Game Boy bindings
  struct Hook {
    virtual void lcdScanline() {}
    virtual void lcdOutput(uint2 color) {}
    virtual void joypWrite(bool p15, bool p14) {}
  };
  Hook* hook = nullptr;

  void lcdScanline();
  void lcdOutput(uint2 color);
  void joypWrite(bool p15, bool p14);

  string title();
  double videoFrequency();
  double audioFrequency();

  bool loaded();
  string sha256();
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

  void cheatSet(const lstring&);

  void paletteUpdate(PaletteMode mode);

  Interface();

private:
  vector<Device> device;
};

extern Interface* interface;

#ifndef GB_HPP
}
#endif
