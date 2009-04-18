class Cartridge : public property {
public:
  enum Mode {
    ModeNormal,
    ModeBsxSlotted,
    ModeBsx,
    ModeSufamiTurbo,
  };

  enum Type {
    TypeNormal,
    TypeBsxSlotted,
    TypeBsxBios,
    TypeBsx,
    TypeSufamiTurboBios,
    TypeSufamiTurbo,
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

  property_t<bool>   loaded;            //is a base cartridge inserted?
  property_t<bool>   bsx_flash_loaded;  //is a BS-X flash cart connected?
  property_t<bool>   patched;           //has a UPS patch been applied?
  property_t<string> name;              //display name (filename sans path and extension)

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
  bool load_normal      (const char *base);
  bool load_bsx_slotted (const char *base, const char *slot  = "");
  bool load_bsx         (const char *base, const char *slot  = "");
  bool load_sufami_turbo(const char *base, const char *slotA = "", const char *slotB = "");
  void unload();

  //utility functions
  static string filepath(const char *filename, const char *pathname);  //"./bar.ext" -> "/foo/bar.ext"
  static string basename(const char *filename);                        //"/foo/bar.ext" -> "bar"
  static string basepath(const char *filename);                        //"/foo/bar.ext" -> "/foo/bar/"
  //this function will load 'filename', decompress it if needed, and determine what type of
  //image file 'filename' refers to (eg normal cart, BS-X flash cart, Sufami Turbo cart, etc.)
  //warning: this operation is very expensive, use sparingly!
  Type detect_image_type(const char *filename) const;

  Cartridge();
  ~Cartridge();

private:
  void load_begin(Mode);
  void load_end();
  void unload_normal();
  void unload_bsx_slotted();
  void unload_bsx();
  void unload_sufami_turbo();

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

  bool load_image(const char *filename, uint8_t *&data, unsigned &size, bool &patched) const;
  bool load_ram  (const char *filename, uint8_t *&data, unsigned  size, uint8_t init_value) const;

  enum CompressionMode {
    CompressionNone,     //always load without compression
    CompressionInspect,  //use file header inspection
    CompressionAuto,     //use file extension or file header inspection (configured by user)
  };

  bool load_file(const char *fn, uint8 *&data, unsigned &size, CompressionMode compression = CompressionNone) const;
  bool save_file(const char *fn, uint8 *data, unsigned size) const;
  bool apply_patch(const uint8_t *pdata, unsigned psize, uint8_t *&data, unsigned &size) const;

  string modify_extension(const char *filename, const char *extension) const;
  string get_filename(const char *source, const char *extension, const char *path) const;

  struct {
    string filename;
    uint8_t *rom, *ram, *rtc;
    unsigned rom_size, ram_size, rtc_size;
  } cart;

  struct {
    string filename;
    uint8_t *ram;
    unsigned ram_size;
  } bs;

  struct {
    string filename;
    uint8_t *rom, *ram;
    unsigned rom_size, ram_size;
  } stA, stB;
};

namespace memory {
  extern MappedRAM cartrom, cartram, cartrtc;
  extern MappedRAM bscram;
  extern MappedRAM stArom, stAram;
  extern MappedRAM stBrom, stBram;
};

extern Cartridge cartridge;
