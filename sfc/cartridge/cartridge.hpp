struct Cartridge : property<Cartridge> {
  enum class Region : unsigned { NTSC, PAL };

  MappedRAM rom;
  MappedRAM ram;

  auto loaded() const -> bool { return _loaded; }
  auto sha256() const -> string { return _sha256; }
  auto region() const -> Region { return _region; }

  readonly<bool> hasICD2;
  readonly<bool> hasMCC;
  readonly<bool> hasNSSDIP;
  readonly<bool> hasEvent;
  readonly<bool> hasSA1;
  readonly<bool> hasSuperFX;
  readonly<bool> hasARMDSP;
  readonly<bool> hasHitachiDSP;
  readonly<bool> hasNECDSP;
  readonly<bool> hasEpsonRTC;
  readonly<bool> hasSharpRTC;
  readonly<bool> hasSPC7110;
  readonly<bool> hasSDD1;
  readonly<bool> hasOBC1;
  readonly<bool> hasMSU1;

  readonly<bool> hasSuperGameBoySlot;
  readonly<bool> hasSatellaviewSlot;
  readonly<bool> hasSufamiTurboSlots;

  struct Mapping {
    function<auto (uint, uint8) -> uint8> reader;
    function<auto (uint, uint8) -> void> writer;
    string addr;
    uint size = 0;
    uint base = 0;
    uint mask = 0;

    Mapping() = default;
    Mapping(const function<uint8 (uint, uint8)>&, const function<void (uint, uint8)>&);
    Mapping(SuperFamicom::Memory&);
  };
  vector<Mapping> mapping;

  struct Memory {
    unsigned id;
    string name;
  };
  vector<Memory> memory;

  struct Information {
    struct Markup {
      string cartridge;
      string gameBoy;
      string satellaview;
      string sufamiTurboA;
      string sufamiTurboB;
    } markup;

    struct Title {
      string cartridge;
      string gameBoy;
      string satellaview;
      string sufamiTurboA;
      string sufamiTurboB;
    } title;
  } information;

  Cartridge() = default;
  ~Cartridge() { unload(); }

  auto title() -> string;

  auto load() -> void;
  auto unload() -> void;

  auto serialize(serializer&) -> void;

private:
  auto loadSuperGameBoy() -> void;
  auto loadSatellaview() -> void;
  auto loadSufamiTurboA() -> void;
  auto loadSufamiTurboB() -> void;
  friend class Interface;

  //markup.cpp
  auto parseMarkup(const string&) -> void;
  auto parseMarkupMap(Mapping&, Markup::Node) -> void;
  auto parseMarkupMemory(MappedRAM&, Markup::Node, unsigned id, bool writable) -> void;

  auto parseMarkupCartridge(Markup::Node) -> void;
  auto parseMarkupICD2(Markup::Node) -> void;
  auto parseMarkupMCC(Markup::Node) -> void;
  auto parseMarkupSatellaview(Markup::Node) -> void;
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

  bool _loaded = false;
  string _sha256;
  Region _region = Region::NTSC;
};

extern Cartridge cartridge;
