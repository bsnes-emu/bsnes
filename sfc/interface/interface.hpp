#ifndef SFC_HPP
namespace SuperFamicom {
#endif

struct ID {
  enum : uint {
    //cartridges (folders)
    System,
    SuperFamicom,
    SuperGameBoy,
    Satellaview,
    SufamiTurboSlotA,
    SufamiTurboSlotB,

    //memory (files)
    SystemManifest,
    IPLROM,

    Manifest,
    ROM,
    RAM,

    EventROM0,
    EventROM1,
    EventROM2,
    EventROM3,
    EventRAM,

    SA1ROM,
    SA1IRAM,
    SA1BWRAM,

    SuperFXROM,
    SuperFXRAM,

    ArmDSPPROM,
    ArmDSPDROM,
    ArmDSPRAM,

    HitachiDSPROM,
    HitachiDSPRAM,
    HitachiDSPDROM,
    HitachiDSPDRAM,

    Nec7725DSPPROM,
    Nec7725DSPDROM,
    Nec7725DSPRAM,

    Nec96050DSPPROM,
    Nec96050DSPDROM,
    Nec96050DSPRAM,

    EpsonRTC,
    SharpRTC,

    SPC7110PROM,
    SPC7110DROM,
    SPC7110RAM,

    SDD1ROM,
    SDD1RAM,

    OBC1RAM,

    SuperGameBoyBootROM,

    MCCROM,
    MCCRAM,
    MCCPSRAM,

    SuperGameBoyManifest,
    SuperGameBoyROM,
    SuperGameBoyRAM,

    SatellaviewManifest,
    SatellaviewROM,

    SufamiTurboSlotAManifest,
    SufamiTurboSlotAROM,
    SufamiTurboSlotARAM,

    SufamiTurboSlotBManifest,
    SufamiTurboSlotBROM,
    SufamiTurboSlotBRAM,

    //device ports (bitmask)
    ControllerPort1 = 1,
    ControllerPort2 = 2,
    ExpansionPort   = 4,
  };
};

struct Interface : Emulator::Interface {
  Interface();

  auto title() -> string;
  auto videoFrequency() -> double;
  auto audioFrequency() -> double;

  auto loaded() -> bool;
  auto sha256() -> string;
  auto group(uint id) -> uint;
  auto load(uint id) -> void;
  auto save() -> void;
  auto load(uint id, const stream& stream) -> void;
  auto save(uint id, const stream& stream) -> void;
  auto unload() -> void;

  auto connect(uint port, uint device) -> void;
  auto power() -> void;
  auto reset() -> void;
  auto run() -> void;

  auto rtc() -> bool;
  auto rtcsync() -> void;

  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto cheatSet(const lstring&) -> void;

  auto paletteUpdate(PaletteMode mode) -> void;

  vector<Device> device;
};

extern Interface* interface;

#ifndef SFC_HPP
}
#endif
