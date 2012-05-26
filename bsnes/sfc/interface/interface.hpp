#ifndef SFC_HPP
namespace SuperFamicom {
#endif

struct ID {
  enum : unsigned {
    SuperFamicom,
  };

  enum : unsigned {
    IPLROM,

    ROM,
    RAM,

    ArmDSP,
    HitachiDSP,
    Nec7725DSP,
    Nec96050DSP,
    NecDSPRAM,

    EpsonRTC,
    SharpRTC,

    SuperGameBoyROM,
    SuperGameBoyRAM,

    BsxFlashROM,
    BsxPSRAM,

    SufamiTurboSlotAROM,
    SufamiTurboSlotBROM,
    SufamiTurboSlotARAM,
    SufamiTurboSlotBRAM,
  };

  enum : unsigned {
    Port1 = 1,
    Port2 = 2,
  };
};

struct Interface : Emulator::Interface {
  double videoFrequency();
  double audioFrequency();

  bool loaded();
  string sha256();
  unsigned group(unsigned id);
  void load(unsigned id, const string &manifest);
  void save();
  void load(unsigned id, const stream &stream, const string &markup = "");
  void save(unsigned id, const stream &stream);
  void unload();

  void connect(unsigned port, unsigned device);
  void power();
  void reset();
  void run();

  bool rtc();
  void rtcsync();

  serializer serialize();
  bool unserialize(serializer&);

  void cheatSet(const lstring&);

  void paletteUpdate();

  bool tracerEnable(bool);

  Interface();

  file tracer;
  vector<Device> device;
};

extern Interface *interface;

#ifndef SFC_HPP
}
#endif
