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
  readonly<bool> has_msu1;

  struct Mapping {
    function<uint8 (unsigned)> read;
    function<void (unsigned, uint8)> write;
    Bus::MapMode mode;
    unsigned banklo;
    unsigned bankhi;
    unsigned addrlo;
    unsigned addrhi;
    unsigned offset;
    unsigned size;

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
  void parse_markup_map(Mapping&, XML::Node&);
  void parse_markup_memory(MappedRAM&, XML::Node&, unsigned id, bool writable);

  void parse_markup_cartridge(XML::Node&);
  void parse_markup_icd2(XML::Node&);
  void parse_markup_bsx(XML::Node&);
  void parse_markup_bsxslot(XML::Node&);
  void parse_markup_sufamiturbo(XML::Node&);
  void parse_markup_nss(XML::Node&);
  void parse_markup_sa1(XML::Node&);
  void parse_markup_superfx(XML::Node&);
  void parse_markup_armdsp(XML::Node&);
  void parse_markup_hitachidsp(XML::Node&);
  void parse_markup_necdsp(XML::Node&);
  void parse_markup_epsonrtc(XML::Node&);
  void parse_markup_sharprtc(XML::Node&);
  void parse_markup_spc7110(XML::Node&);
  void parse_markup_sdd1(XML::Node&);
  void parse_markup_obc1(XML::Node&);
  void parse_markup_msu1(XML::Node&);
};

extern Cartridge cartridge;
