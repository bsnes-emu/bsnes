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

  enum class SuperGameBoyVersion : unsigned {
    Version1,
    Version2,
  };

  //assigned externally to point to file-system datafiles (msu1 and serial)
  //example: "/path/to/filename.sfc" would set this to "/path/to/filename"
  readwrite<string> basename;

  readonly<bool> loaded;
  readonly<unsigned> crc32;
  readonly<string> sha256;

  readonly<Mode> mode;
  readonly<Region> region;
  readonly<unsigned> ram_size;
  readonly<unsigned> spc7110_data_rom_offset;
  readonly<SuperGameBoyVersion> supergameboy_version;

  readonly<bool> has_bsx_slot;
  readonly<bool> has_superfx;
  readonly<bool> has_sa1;
  readonly<bool> has_upd7725;
  readonly<bool> has_upd96050;
  readonly<bool> has_srtc;
  readonly<bool> has_sdd1;
  readonly<bool> has_spc7110;
  readonly<bool> has_spc7110rtc;
  readonly<bool> has_cx4;
  readonly<bool> has_obc1;
  readonly<bool> has_st0010;
  readonly<bool> has_st0011;
  readonly<bool> has_st0018;
  readonly<bool> has_msu1;
  readonly<bool> has_serial;

  struct Mapping {
    Memory *memory;
    MMIO *mmio;
    Bus::MapMode mode;
    unsigned banklo;
    unsigned bankhi;
    unsigned addrlo;
    unsigned addrhi;
    unsigned offset;
    unsigned size;

    Mapping();
    Mapping(Memory&);
    Mapping(MMIO&);
  };
  array<Mapping> mapping;

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
  void xml_parse_icd2(xml_element&);
  void xml_parse_superfx(xml_element&);
  void xml_parse_sa1(xml_element&);
  void xml_parse_upd7725(xml_element&);
  void xml_parse_upd96050(xml_element&);
  void xml_parse_bsx(xml_element&);
  void xml_parse_sufamiturbo(xml_element&);
  void xml_parse_supergameboy(xml_element&);
  void xml_parse_srtc(xml_element&);
  void xml_parse_sdd1(xml_element&);
  void xml_parse_spc7110(xml_element&);
  void xml_parse_cx4(xml_element&);
  void xml_parse_necdsp(xml_element&);
  void xml_parse_obc1(xml_element&);
  void xml_parse_setadsp(xml_element&);
  void xml_parse_setarisc(xml_element&);
  void xml_parse_msu1(xml_element&);
  void xml_parse_serial(xml_element&);

  void xml_parse_address(Mapping&, const string&);
  void xml_parse_mode(Mapping&, const string&);
};

namespace memory {
  extern MappedRAM cartrom, cartram, cartrtc;
  extern MappedRAM bsxflash, bsxram, bsxpram;
  extern MappedRAM stArom, stAram;
  extern MappedRAM stBrom, stBram;
};

extern Cartridge cartridge;
