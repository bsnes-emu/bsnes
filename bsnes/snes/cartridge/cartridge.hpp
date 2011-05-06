class Cartridge : property<Cartridge> {
public:
  enum class Mode : unsigned {
    Normal,
    BsxSlotted,
    Bsx,
    SufamiTurbo,
    SuperGameBoy,
  };

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
  readonly<unsigned> crc32;
  readonly<string> sha256;

  readonly<Mode> mode;
  readonly<Region> region;
  readonly<unsigned> ram_size;

  readonly<bool> has_bsx_slot;
  readonly<bool> has_nss_dip;
  readonly<bool> has_superfx;
  readonly<bool> has_sa1;
  readonly<bool> has_necdsp;
  readonly<bool> has_srtc;
  readonly<bool> has_sdd1;
  readonly<bool> has_spc7110;
  readonly<bool> has_spc7110rtc;
  readonly<bool> has_cx4;
  readonly<bool> has_obc1;
  readonly<bool> has_st0018;
  readonly<bool> has_msu1;
  readonly<bool> has_serial;

  struct NonVolatileRAM {
    const string id;
    uint8_t *data;
    unsigned size;
    Slot slot;
    NonVolatileRAM() : id(""), data(0), size(0), slot(Slot::Base) {}
    NonVolatileRAM(const string id, uint8_t *data, unsigned size, Slot slot = Slot::Base)
    : id(id), data(data), size(size), slot(slot) {}
  };
  linear_vector<NonVolatileRAM> nvram;

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
    Mapping(Memory&);
  };
  linear_vector<Mapping> mapping;

  struct Information {
    struct NSS {
      lstring setting;
      lstring option[16];
    } nss;
  } information;

  void load(Mode, const lstring&);
  void unload();

  void serialize(serializer&);
  Cartridge();
  ~Cartridge();

private:
  void parse_xml(const lstring&);
  void parse_xml_cartridge(const char*);
  void parse_xml_bsx(const char*);
  void parse_xml_sufami_turbo(const char*, bool);
  void parse_xml_gameboy(const char*);

  void xml_parse_rom(xml_element&);
  void xml_parse_ram(xml_element&);
  void xml_parse_nss(xml_element&);
  void xml_parse_icd2(xml_element&);
  void xml_parse_superfx(xml_element&);
  void xml_parse_sa1(xml_element&);
  void xml_parse_necdsp(xml_element&);
  void xml_parse_bsx(xml_element&);
  void xml_parse_sufamiturbo(xml_element&);
  void xml_parse_supergameboy(xml_element&);
  void xml_parse_srtc(xml_element&);
  void xml_parse_sdd1(xml_element&);
  void xml_parse_spc7110(xml_element&);
  void xml_parse_cx4(xml_element&);
  void xml_parse_obc1(xml_element&);
  void xml_parse_setarisc(xml_element&);
  void xml_parse_msu1(xml_element&);
  void xml_parse_serial(xml_element&);

  void xml_parse_address(Mapping&, const string&);
  void xml_parse_mode(Mapping&, const string&);
};

extern Cartridge cartridge;
