class Cartridge : public property {
public:
  enum Mode {
    ModeNormal,
    ModeBsxSlotted,
    ModeBsx,
    ModeSufamiTurbo,
    ModeSuperGameboy,
  };

  enum Type {
    TypeNormal,
    TypeBsxSlotted,
    TypeBsxBios,
    TypeBsx,
    TypeSufamiTurboBios,
    TypeSufamiTurbo,
    TypeSuperGameboyBios,
    TypeGameboy,
    TypeUnknown,
  };

  enum Region {
    NTSC,
    PAL,
  };

  enum MemoryMapper {
    LoROM,
    HiROM,
    ExLoROM,
    ExHiROM,
    SA1ROM,
    SPC7110ROM,
    BSCLoROM,
    BSCHiROM,
    BSXROM,
    STROM,
  };

  enum DSP1MemoryMapper {
    DSP1Unmapped,
    DSP1LoROM1MB,
    DSP1LoROM2MB,
    DSP1HiROM,
  };

  //properties can be read via operator(), eg "if(cartridge.loaded() == true)";
  //warning: if loaded() == false, no other property is considered valid!

  property_t<bool> loaded;            //is a base cartridge inserted?
  property_t<bool> bsx_flash_loaded;  //is a BS-X flash cart connected?
  property_t<bool> patched;           //has a UPS patch been applied?

  property_t<Mode>             mode;
  property_t<Region>           region;
  property_t<MemoryMapper>     mapper;
  property_t<DSP1MemoryMapper> dsp1_mapper;

  property_t<bool> has_bsx_slot;
  property_t<bool> has_superfx;
  property_t<bool> has_sa1;
  property_t<bool> has_srtc;
  property_t<bool> has_sdd1;
  property_t<bool> has_spc7110, has_spc7110rtc;
  property_t<bool> has_cx4;
  property_t<bool> has_dsp1, has_dsp2, has_dsp3, has_dsp4;
  property_t<bool> has_obc1;
  property_t<bool> has_st010, has_st011, has_st018;

  //main interface
  Type detect_image_type (uint8_t*, unsigned) const;
  bool load_normal       (uint8_t*, unsigned);
  bool load_bsx_slotted  (uint8_t*, unsigned, uint8_t*, unsigned);
  bool load_bsx          (uint8_t*, unsigned, uint8_t*, unsigned);
  bool load_sufami_turbo (uint8_t*, unsigned, uint8_t*, unsigned, uint8_t*, unsigned);
  bool load_super_gameboy(uint8_t*, unsigned, uint8_t*, unsigned);
  void unload();

  Cartridge();
  ~Cartridge();

private:
  void load_begin(Mode);
  void load_end();

  struct cartinfo_t {
    Type type;
    Region region;
    MemoryMapper mapper;
    DSP1MemoryMapper dsp1_mapper;
    unsigned rom_size, ram_size;

    bool bsx_slot;
    bool superfx;
    bool sa1;
    bool srtc;
    bool sdd1;
    bool spc7110, spc7110rtc;
    bool cx4;
    bool dsp1, dsp2, dsp3, dsp4;
    bool obc1;
    bool st010, st011, st018;

    void reset();
    cartinfo_t();
  };

  enum HeaderField {
    CartName    = 0x00,
    Mapper      = 0x15,
    RomType     = 0x16,
    RomSize     = 0x17,
    RamSize     = 0x18,
    CartRegion  = 0x19,
    Company     = 0x1a,
    Version     = 0x1b,
    Complement  = 0x1c,  //inverse checksum
    Checksum    = 0x1e,
    ResetVector = 0x3c,
  };

  void read_header(cartinfo_t &info, const uint8_t *data, unsigned size) const;
  unsigned find_header(const uint8_t *data, unsigned size) const;
  unsigned score_header(const uint8_t *data, unsigned size, unsigned addr) const;
  void set_cartinfo(const cartinfo_t&);
};

namespace memory {
  extern MappedRAM cartrom, cartram, cartrtc;
  extern MappedRAM bscram;
  extern MappedRAM stArom, stAram;
  extern MappedRAM stBrom, stBram;
  extern MappedRAM dmgrom, dmgram, dmgrtc;
};

extern Cartridge cartridge;
