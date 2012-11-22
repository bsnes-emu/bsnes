struct Cartridge : property<Cartridge> {
  enum class Region : unsigned {
    NTSC,
    PAL,
  };

  enum class Slot : unsigned {
    Base,
    Bsx,
    SufamiTurbo,
    SufamiTurboA,
    SufamiTurboB,
    GameBoy,
  };

  MappedRAM rom;
  MappedRAM ram;

  readonly<bool> loaded;
  readonly<string> sha256;
  readonly<string> manifest;

  readonly<Region> region;

  readonly<bool> has_gb_slot;
  readonly<bool> has_bs_cart;
  readonly<bool> has_bs_slot;
  readonly<bool> has_st_slots;
  readonly<bool> has_nss_dip;
  readonly<bool> has_event;
  readonly<bool> has_sa1;
  readonly<bool> has_superfx;
  readonly<bool> has_armdsp;
  readonly<bool> has_hitachidsp;
  readonly<bool> has_necdsp;
  readonly<bool> has_epsonrtc;
  readonly<bool> has_sharprtc;
  readonly<bool> has_spc7110;
  readonly<bool> has_sdd1;
  readonly<bool> has_obc1;
  readonly<bool> has_hsu1;
  readonly<bool> has_msu1;

  struct Mapping {
    function<uint8 (unsigned)> reader;
    function<void (unsigned, uint8)> writer;
    string addr;
    unsigned size;
    unsigned base;
    unsigned mask;

    Mapping();
    Mapping(const function<uint8 (unsigned)>&, const function<void (unsigned, uint8)>&);
    Mapping(SuperFamicom::Memory&);
  };
  vector<Mapping> mapping;

  struct Memory {
    unsigned id;
    string name;
  };
  vector<Memory> memory;

  void load(const string &manifest);
  void load_super_game_boy(const string &manifest);
  void load_satellaview(const string &manifest);
  void load_sufami_turbo_a(const string &manifest);
  void load_sufami_turbo_b(const string &manifest);
  void unload();

  void serialize(serializer&);
  Cartridge();
  ~Cartridge();

private:
  void parse_markup(const char*);
  void parse_markup_map(Mapping&, Markup::Node);
  void parse_markup_memory(MappedRAM&, Markup::Node, unsigned id, bool writable);

  void parse_markup_cartridge(Markup::Node);
  void parse_markup_icd2(Markup::Node);
  void parse_markup_bsx(Markup::Node);
  void parse_markup_bsxslot(Markup::Node);
  void parse_markup_sufamiturbo(Markup::Node);
  void parse_markup_nss(Markup::Node);
  void parse_markup_event(Markup::Node);
  void parse_markup_sa1(Markup::Node);
  void parse_markup_superfx(Markup::Node);
  void parse_markup_armdsp(Markup::Node);
  void parse_markup_hitachidsp(Markup::Node, unsigned roms);
  void parse_markup_necdsp(Markup::Node);
  void parse_markup_epsonrtc(Markup::Node);
  void parse_markup_sharprtc(Markup::Node);
  void parse_markup_spc7110(Markup::Node);
  void parse_markup_sdd1(Markup::Node);
  void parse_markup_obc1(Markup::Node);
  void parse_markup_hsu1(Markup::Node);
  void parse_markup_msu1(Markup::Node);
};

extern Cartridge cartridge;
