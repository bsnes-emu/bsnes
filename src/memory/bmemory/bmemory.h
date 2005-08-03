#include "bcart_lorom.h"
#include "bcart_hirom.h"
#include "bcart_exhirom.h"

class bMemBus : public MemBus {
public:
uint8 *rom, *sram, *wram;
uint32 rom_size, sram_size;

bool rom_loaded;
enum { LOROM = 0x20, HIROM = 0x21, EXHIROM = 0x25 };

  uint8 read (uint32 addr);
  void  write(uint32 addr, byte value);

  bool  load_cart(Reader *rf);
  bool  load_sram(Reader *rf);
  bool  save_sram(Writer *wf);
  void  unload_cart();
  void  get_cartinfo(CartInfo *ci);

  void power();
  void reset();

  bMemBus();
  ~bMemBus();
};
