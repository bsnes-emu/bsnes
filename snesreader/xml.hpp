class XML {
public:
  void generate(nall::string &xml, const uint8_t *data, unsigned size);

private:
  void read_header(const uint8_t *data, unsigned size);
  unsigned find_header(const uint8_t *data, unsigned size) const;
  unsigned score_header(const uint8_t *data, unsigned size, unsigned addr) const;

  unsigned gameboy_ram_size(const uint8_t *data, unsigned size) const;
  bool gameboy_has_rtc(const uint8_t *data, unsigned size) const;

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

  enum Mode {
    ModeNormal,
    ModeBsxSlotted,
    ModeBsx,
    ModeSufamiTurbo,
    ModeSuperGameBoy,
  };

  enum Type {
    TypeNormal,
    TypeBsxSlotted,
    TypeBsxBios,
    TypeBsx,
    TypeSufamiTurboBios,
    TypeSufamiTurbo,
    TypeSuperGameBoy1Bios,
    TypeSuperGameBoy2Bios,
    TypeGameBoy,
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
    SuperFXROM,
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

  bool loaded;        //is a base cartridge inserted?
  unsigned crc32;     //crc32 of all cartridges (base+slot(s))
  unsigned rom_size;
  unsigned ram_size;

  Mode mode;
  Type type;
  Region region;
  MemoryMapper mapper;
  DSP1MemoryMapper dsp1_mapper;

  bool has_bsx_slot;
  bool has_superfx;
  bool has_sa1;
  bool has_srtc;
  bool has_sdd1;
  bool has_spc7110;
  bool has_spc7110rtc;
  bool has_cx4;
  bool has_dsp1;
  bool has_dsp2;
  bool has_dsp3;
  bool has_dsp4;
  bool has_obc1;
  bool has_st010;
  bool has_st011;
  bool has_st018;
};

extern XML xml;
