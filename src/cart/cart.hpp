class Cartridge {
public:
  enum CartridgeMode {
    ModeNormal,
    ModeBSC,
    ModeBSX,
    ModeSufamiTurbo,
  };

  enum CartridgeType {
    TypeNormal,
    TypeBSC,
    TypeBSXBIOS,
    TypeBSX,
    TypeSufamiTurboBIOS,
    TypeSufamiTurbo,
    TypeUnknown,
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

  enum Region {
    NTSC,
    PAL,
  };

  enum MemoryMapper {
    LoROM,
    HiROM,
    ExLoROM,
    ExHiROM,
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

  const char* name();
  CartridgeMode mode();
  MemoryMapper mapper();
  Region region();

  struct {
    bool loaded;
    char fn[PATH_MAX];
    uint8 *rom, *ram, *rtc;
    uint rom_size, ram_size, rtc_size;
  } cart;

  struct {
    char fn[PATH_MAX];
    uint8 *ram;
    uint ram_size;
  } bs;

  struct {
    char fn[PATH_MAX];
    uint8 *rom, *ram;
    uint rom_size, ram_size;
  } stA, stB;

  struct cartinfo_t {
    CartridgeType type;
    MemoryMapper mapper;
    DSP1MemoryMapper dsp1_mapper;
    Region region;

    unsigned rom_size;
    unsigned ram_size;

    bool bsxslot;
    bool superfx;
    bool sa1;
    bool srtc;
    bool sdd1;
    bool spc7110;
    bool spc7110rtc;
    bool cx4;
    bool dsp1;
    bool dsp2;
    bool dsp3;
    bool dsp4;
    bool obc1;
    bool st010;
    bool st011;
    bool st018;

    void reset();
  };

  struct info_t {
    char filename[PATH_MAX * 4];
    bool patched;

    CartridgeMode mode;
    MemoryMapper mapper;
    DSP1MemoryMapper dsp1_mapper;
    Region region;

    bool bsxcart;   //is BS-X cart inserted?
    bool bsxflash;  //is BS-X flash cart inserted into BS-X cart?

    bool bsxslot;
    bool superfx;
    bool sa1;
    bool srtc;
    bool sdd1;
    bool spc7110;
    bool spc7110rtc;
    bool cx4;
    bool dsp1;
    bool dsp2;
    bool dsp3;
    bool dsp4;
    bool obc1;
    bool st010;
    bool st011;
    bool st018;

    info_t& operator=(const cartinfo_t&);
  } info;

  struct {
    char fn[PATH_MAX];
    uint8_t *data;
    unsigned size;
  } image;
  bool load_image(const char*);
  bool inspect_image(cartinfo_t &cartinfo, const char *filename);
  bool load_ram(const char *filename, uint8_t *&data, unsigned size, uint8_t init);

  void load_cart_normal(const char*);
  void load_cart_bsc(const char*, const char*);
  void load_cart_bsx(const char*, const char*);
  void load_cart_st(const char*, const char*, const char*);

  void unload_cart_normal();
  void unload_cart_bsx();
  void unload_cart_bsc();
  void unload_cart_st();

  bool loaded();
  void load_begin(CartridgeMode);
  void load_end();
  bool unload();

  void read_header(cartinfo_t &info, const uint8_t *data, unsigned size);
  unsigned find_header(const uint8_t *data, unsigned size);
  unsigned score_header(const uint8_t *data, unsigned size, unsigned addr);

  enum CompressionMode {
    CompressionNone,    //always load without compression
    CompressionInspect, //use file header inspection
    CompressionAuto,    //use file extension or file header inspection (configured by user)
  };
  bool load_file(const char *fn, uint8 *&data, uint &size, CompressionMode compression = CompressionNone);
  bool save_file(const char *fn, uint8 *data, uint size);
  bool apply_patch(const uint8_t *pdata, unsigned psize, uint8_t *&data, unsigned &size);
  char* modify_extension(char *filename, const char *extension);
  char* get_base_filename(char *filename);
  char* get_path_filename(char *filename, const char *path, const char *source, const char *extension);
  char* get_patch_filename(const char *source, const char *extension);
  char* get_save_filename(const char *source, const char *extension);
  char* get_cheat_filename(const char *source, const char *extension);

  Cartridge();
  ~Cartridge();

private:
  char patchfn[PATH_MAX];
  char savefn[PATH_MAX];
  char rtcfn[PATH_MAX];
  char cheatfn[PATH_MAX];
};

namespace memory {
  extern MappedRAM cartrom, cartram, cartrtc;
  extern MappedRAM bscram;
  extern MappedRAM stArom, stAram;
  extern MappedRAM stBrom, stBram;
};

extern Cartridge cartridge;
