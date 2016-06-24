struct Cartridge {
  enum class Region : uint { NTSC, PAL };

  auto pathID() const -> uint { return information.pathID; }
  auto sha256() const -> string { return information.sha256; }
  auto region() const -> Region { return information.region; }
  auto manifest() const -> string;
  auto title() const -> string;

  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;

  auto serialize(serializer&) -> void;

  MappedRAM rom;
  MappedRAM ram;

  struct Information {
    uint pathID = 0;
    string sha256;
    Region region = Region::NTSC;

    struct Manifest {
      string cartridge;
      string gameBoy;
      string bsMemory;
      string sufamiTurboA;
      string sufamiTurboB;
    } manifest;

    struct Title {
      string cartridge;
      string gameBoy;
      string bsMemory;
      string sufamiTurboA;
      string sufamiTurboB;
    } title;
  } information;

  struct Has {
    boolean ICD2;
    boolean MCC;
    boolean NSSDIP;
    boolean Event;
    boolean SA1;
    boolean SuperFX;
    boolean ARMDSP;
    boolean HitachiDSP;
    boolean NECDSP;
    boolean EpsonRTC;
    boolean SharpRTC;
    boolean SPC7110;
    boolean SDD1;
    boolean OBC1;
    boolean MSU1;

    boolean GameBoySlot;
    boolean BSMemorySlot;
    boolean SufamiTurboSlots;
  } has;

private:
  //cartridge.cpp
  auto loadGameBoy() -> bool;
  auto loadBSMemory() -> bool;
  auto loadSufamiTurboA() -> bool;
  auto loadSufamiTurboB() -> bool;

  //load.cpp
  auto loadCartridge(Markup::Node) -> void;
  auto loadGameBoy(Markup::Node) -> void;
  auto loadBSMemory(Markup::Node) -> void;
  auto loadSufamiTurboA(Markup::Node) -> void;
  auto loadSufamiTurboB(Markup::Node) -> void;

  auto loadROM(Markup::Node) -> void;
  auto loadRAM(Markup::Node) -> void;
  auto loadICD2(Markup::Node) -> void;
  auto loadMCC(Markup::Node) -> void;
  auto loadBSMemoryPack(Markup::Node) -> void;
  auto loadSufamiTurbo(Markup::Node, bool slot) -> void;
  auto loadNSS(Markup::Node) -> void;
  auto loadEvent(Markup::Node) -> void;
  auto loadSA1(Markup::Node) -> void;
  auto loadSuperFX(Markup::Node) -> void;
  auto loadARMDSP(Markup::Node) -> void;
  auto loadHitachiDSP(Markup::Node, uint roms) -> void;
  auto loadNECDSP(Markup::Node) -> void;
  auto loadEpsonRTC(Markup::Node) -> void;
  auto loadSharpRTC(Markup::Node) -> void;
  auto loadSPC7110(Markup::Node) -> void;
  auto loadSDD1(Markup::Node) -> void;
  auto loadOBC1(Markup::Node) -> void;
  auto loadMSU1(Markup::Node) -> void;

  auto loadMemory(MappedRAM&, Markup::Node, bool required, maybe<uint> id = nothing) -> void;
  auto loadMap(Markup::Node, SuperFamicom::Memory&) -> void;
  auto loadMap(Markup::Node, const function<uint8 (uint24, uint8)>&, const function<void (uint24, uint8)>&) -> void;

  //save.cpp
  auto saveCartridge(Markup::Node) -> void;
  auto saveGameBoy(Markup::Node) -> void;
  auto saveBSMemory(Markup::Node) -> void;
  auto saveSufamiTurboA(Markup::Node) -> void;
  auto saveSufamiTurboB(Markup::Node) -> void;

  auto saveRAM(Markup::Node) -> void;
  auto saveMCC(Markup::Node) -> void;
  auto saveEvent(Markup::Node) -> void;
  auto saveSA1(Markup::Node) -> void;
  auto saveSuperFX(Markup::Node) -> void;
  auto saveARMDSP(Markup::Node) -> void;
  auto saveHitachiDSP(Markup::Node) -> void;
  auto saveNECDSP(Markup::Node) -> void;
  auto saveEpsonRTC(Markup::Node) -> void;
  auto saveSharpRTC(Markup::Node) -> void;
  auto saveSPC7110(Markup::Node) -> void;
  auto saveSDD1(Markup::Node) -> void;
  auto saveOBC1(Markup::Node) -> void;

  auto saveMemory(MappedRAM&, Markup::Node, maybe<uint> = nothing) -> void;

  friend class Interface;
  friend class ICD2;
};

extern Cartridge cartridge;
