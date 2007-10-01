class Cartridge {
public:

/*****
 * cart database
 *****/

#include "db/db.h"
db_item dbi;
  uint8 *database;
  uint   database_size;
  uint   database_blocksize;
  void   load_database();
  bool   read_database();

//

enum {
  CART_NORMAL,
  CART_ST,
  CART_STDUAL,
};

bool cart_loaded;

uint8 rom_header[512], *rom, *ram;

enum {
//header fields
  CART_NAME = 0x00,
  MAPPER    = 0x15,
  ROM_TYPE  = 0x16,
  ROM_SIZE  = 0x17,
  RAM_SIZE  = 0x18,
  REGION    = 0x19,
  LICENSE   = 0x1a,
  VERSION   = 0x1b,
  ICKSUM    = 0x1c,
  CKSUM     = 0x1e,
  RESL      = 0x3c,
  RESH      = 0x3d,

//regions
  NTSC = 0,
  PAL  = 1,

//memory mappers
  PCB     = 0x00,
  LOROM   = 0x20,
  HIROM   = 0x21,
  EXLOROM = 0x22,
  EXHIROM = 0x25,

//special chip memory mappers
  DSP1_LOROM_1MB = 1,
  DSP1_LOROM_2MB = 2,
  DSP1_HIROM     = 3,
};

struct {
  uint   count;
  char   cheat_name[4096], patch_name[4096];
  char   rom_name[8][4096], ram_name[8][4096];
  uint   rom_size[8],  ram_size[8];
  uint8 *rom_data[8], *ram_data[8];
} file;

struct {
  uint   type;

//cart information
  uint32 crc32;
  char   name[128];
  char   pcb[32];

  uint   region;
  uint   mapper;
  uint   rom_size;
  uint   ram_size;

//set to true for games that need cart MMIO mapping (c4, dsp-n, ...),
//for games that map outside the standard MMIO range of $2000-$5fff
  bool   cart_mmio;
  bool   srtc;
  bool   sdd1;
  bool   c4;
  bool   dsp1;
  bool   dsp2;
  bool   obc1;
  bool   st010;

  uint   dsp1_mapper;

//HiROM / LoROM specific code
  uint   header_index;
} info;

  bool load_file(const char *fn, uint8 *&data, uint &size);
  bool save_file(const char *fn, uint8 *data, uint size);

  void load_rom_normal();
  void load_ram_normal();
  void save_ram_normal();

  void load_rom_st();
  void load_ram_st();
  void save_ram_st();

  void load_rom_stdual();
  void load_ram_stdual();
  void save_ram_stdual();

  void find_header();
  void read_header();

  bool loaded() { return cart_loaded; }
  void load_begin(uint cart_type);
  void load(const char *rom_fn);
  bool load_end();
  bool unload();

  Cartridge();
  ~Cartridge();
};

extern Cartridge cartridge;
