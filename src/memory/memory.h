class Memory {
public:
enum { WRAP_NONE = 0, WRAP_BANK = 1, WRAP_PAGE = 2 };
  virtual uint8  read (uint32 addr) = 0;
  virtual void   write(uint32 addr, uint8 value) = 0;
  virtual uint16 read_word (uint32 addr, uint8 wrap = WRAP_NONE);
  virtual void   write_word(uint32 addr, uint16 value, uint8 wrap = WRAP_NONE);
  virtual uint32 read_long (uint32 addr, uint8 wrap = WRAP_NONE);
  virtual void   write_long(uint32 addr, uint32 value, uint8 wrap = WRAP_NONE);
};

typedef struct {
uint8 *rom, *sram;
uint32 rom_size, sram_size;
}CartInfo;

class Cart : public Memory {
public:
  virtual void write_protect(bool r) = 0;
  virtual void set_cartinfo(CartInfo *ci) = 0;
};

class MMIO : public Memory {
public:
  virtual uint8 read (uint32 addr);
  virtual void  write(uint32 addr, uint8 value);
};

class MemBus : public Memory {
public:
Cart *cart;
MMIO *mmio[0x4000];
bool fastROM;
  virtual void  flush_mmio_mappers();
  virtual bool  set_mmio_mapper(uint16 addr, MMIO *mapper);
  virtual uint8 speed(uint32 addr);

  virtual bool  load_cart(Reader *rf) = 0;
  virtual bool  load_sram(Reader *rf) = 0;
  virtual bool  save_sram(Writer *wf) = 0;
  virtual void  unload_cart() = 0;
  virtual void  get_cartinfo(CartInfo *ci) = 0;

  virtual void power() = 0;
  virtual void reset() = 0;

  MemBus();
  ~MemBus();
};
