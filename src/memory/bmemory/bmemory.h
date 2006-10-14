class bMemBus : public MemBus {
public:
uint8 *wram;
bool   rom_loaded;

uint8 *page_handle[65536];
uint8 (bMemBus::*page_read [65536])(uint32 addr);
void  (bMemBus::*page_write[65536])(uint32 addr, uint8 data);

#include "mapper/mapper.h"

enum { LOROM = 0x20, HIROM = 0x21, EXLOROM = 0x22, EXHIROM = 0x25 };

enum { TYPE_WRAM, TYPE_MMIO, TYPE_CART };
  uint8  memory_type(uint32 addr);

  uint8  read (uint32 addr);
  void   write(uint32 addr, uint8 data);

  void   load_cart();
  void   unload_cart();
  bool   cart_loaded() { return rom_loaded; }

//bmemory_rw.cpp
  uint8  read_unmapped (uint32 addr);
  void   write_unmapped(uint32 addr, uint8 data);
  uint8  read_mmio     (uint32 addr);
  void   write_mmio    (uint32 addr, uint8 data);
  uint8  read_rom      (uint32 addr);
  void   write_rom     (uint32 addr, uint8 data);
  uint8  read_ram      (uint32 addr);
  void   write_ram     (uint32 addr, uint8 data);

  uint8  read_sdd1     (uint32 addr);
  void   write_sdd1    (uint32 addr, uint8 data);
  uint8  read_c4       (uint32 addr);
  void   write_c4      (uint32 addr, uint8 data);
  uint8  read_dsp1     (uint32 addr);
  void   write_dsp1    (uint32 addr, uint8 data);
  uint8  read_dsp2     (uint32 addr);
  void   write_dsp2    (uint32 addr, uint8 data);
  uint8  read_obc1     (uint32 addr);
  void   write_obc1    (uint32 addr, uint8 data);

  void   cart_map_reset();
  void   cart_map_system();
  void   cart_map_generic(uint type);

  void   cart_map_sdd1();
  void   cart_map_c4();
  void   cart_map_dsp1();
  void   cart_map_dsp2();
  void   cart_map_obc1();

  void   power();
  void   reset();

//load_cart() helper
  void calc_size(char *t, uint size) {
    size *= 8; //bytes -> bits
    if(size < 1024) { sprintf(t, "%dbit",  size); return; }
    size /= 1024;
    if(size < 1024) { sprintf(t, "%dkbit", size); return; }
    size /= 1024;
    sprintf(t, "%dmbit", size);
  }

  bMemBus();
  ~bMemBus();
};
