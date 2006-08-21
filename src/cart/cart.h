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

bool cart_loaded;
char rom_fn[4096], sram_fn[4096], cheat_fn[4096], patch_fn[4096];

uint8 *base_rom, *rom, *sram;

enum {
//header fields
  CART_NAME = 0x00,
  MAPPER    = 0x15,
  ROM_TYPE  = 0x16,
  ROM_SIZE  = 0x17,
  SRAM_SIZE = 0x18,
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

  uint   dsp1_mapper;

//HiROM / LoROM specific code
  uint   header_index;
} info;

  void load_rom(Reader &rf);
  void load_sram();
  void save_sram();
  void read_dbi();
  void find_header();
  void read_header();
  bool loaded() { return cart_loaded; }
  bool load(const char *fn);
  bool unload();

  Cartridge();
  ~Cartridge();
};

extern Cartridge cartridge;
