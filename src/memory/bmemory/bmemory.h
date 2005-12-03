#include "bcart_lorom.h"
#include "bcart_hirom.h"
#include "bcart_exlorom.h"
#include "bcart_exhirom.h"

class bMemBus : public MemBus {
public:

//rom_image is the actual image, including header.
//rom is the image sans header, which is actually
//just a pointer to rom_image (with no header), or
//rom_image + 512 (if a header is present).
//rom should never be allocated or released directly.
uint8 *wram;

bool rom_loaded;
enum { LOROM = 0x20, HIROM = 0x21, EXLOROM = 0x22, EXHIROM = 0x25 };

  uint8 read (uint32 addr);
  void  write(uint32 addr, uint8 value);

  void load_cart();
  void unload_cart();
  bool cart_loaded() { return rom_loaded; }
  void get_cartinfo(CartInfo *ci);

  void power();
  void reset();

  bMemBus();
  ~bMemBus();
};
