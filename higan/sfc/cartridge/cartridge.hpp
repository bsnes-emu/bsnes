struct Cartridge {
  enum class Region : uint { NTSC, PAL };

  auto sha256() const -> string { return _sha256; }
  auto region() const -> Region { return _region; }

  auto manifest() -> string;
  auto title() -> string;

  auto load() -> void;
  auto unload() -> void;

  auto serialize(serializer&) -> void;

  MappedRAM rom;
  MappedRAM ram;

  struct Memory {
    unsigned id;
    string name;
  };
  vector<Memory> memory;

  struct Information {
    struct Markup {
      string cartridge;
      string gameBoy;
      string bsMemory;
      string sufamiTurboA;
      string sufamiTurboB;
    } markup;

    struct Title {
      string cartridge;
      string gameBoy;
      string bsMemory;
      string sufamiTurboA;
      string sufamiTurboB;
    } title;
  } information;

  bool hasICD2;
  bool hasMCC;
  bool hasNSSDIP;
  bool hasEvent;
  bool hasSA1;
  bool hasSuperFX;
  bool hasARMDSP;
  bool hasHitachiDSP;
  bool hasNECDSP;
  bool hasEpsonRTC;
  bool hasSharpRTC;
  bool hasSPC7110;
  bool hasSDD1;
  bool hasOBC1;
  bool hasMSU1;

  bool hasGameBoySlot;
  bool hasBSMemorySlot;
  bool hasSufamiTurboSlots;

private:
  auto loadGameBoy() -> void;
  auto loadBSMemory() -> void;
  auto loadSufamiTurboA() -> void;
  auto loadSufamiTurboB() -> void;
  friend class Interface;
  friend class ICD2;

  //markup.cpp
  auto parseMarkup(const string&) -> void;
  auto parseMarkupMap(Markup::Node, SuperFamicom::Memory&) -> void;
  auto parseMarkupMap(Markup::Node, const function<uint8 (uint24, uint8)>&, const function<void (uint24, uint8)>&) -> void;
  auto parseMarkupMemory(MappedRAM&, Markup::Node, uint id, bool writable) -> void;

  auto parseMarkupROM(Markup::Node) -> void;
  auto parseMarkupRAM(Markup::Node) -> void;
  auto parseMarkupICD2(Markup::Node) -> void;
  auto parseMarkupMCC(Markup::Node) -> void;
  auto parseMarkupBSMemory(Markup::Node) -> void;
  auto parseMarkupSufamiTurbo(Markup::Node, bool slot) -> void;
  auto parseMarkupNSS(Markup::Node) -> void;
  auto parseMarkupEvent(Markup::Node) -> void;
  auto parseMarkupSA1(Markup::Node) -> void;
  auto parseMarkupSuperFX(Markup::Node) -> void;
  auto parseMarkupARMDSP(Markup::Node) -> void;
  auto parseMarkupHitachiDSP(Markup::Node, unsigned roms) -> void;
  auto parseMarkupNECDSP(Markup::Node) -> void;
  auto parseMarkupEpsonRTC(Markup::Node) -> void;
  auto parseMarkupSharpRTC(Markup::Node) -> void;
  auto parseMarkupSPC7110(Markup::Node) -> void;
  auto parseMarkupSDD1(Markup::Node) -> void;
  auto parseMarkupOBC1(Markup::Node) -> void;
  auto parseMarkupMSU1(Markup::Node) -> void;

  string _sha256;
  Region _region = Region::NTSC;
};

extern Cartridge cartridge;
