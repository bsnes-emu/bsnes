class Memory {
public:
enum { WRAP_NONE = 0, WRAP_BANK = 1, WRAP_PAGE = 2 };
  virtual uint8  read      (uint32 addr)              = 0;
  virtual void   write     (uint32 addr, uint8  data) = 0;
  virtual uint16 read_word (uint32 addr,              uint wrap = WRAP_NONE);
  virtual void   write_word(uint32 addr, uint16 data, uint wrap = WRAP_NONE);
  virtual uint32 read_long (uint32 addr,              uint wrap = WRAP_NONE);
  virtual void   write_long(uint32 addr, uint32 data, uint wrap = WRAP_NONE);
};

typedef struct {
  uint8 *rom, *sram;
  uint32 rom_size, sram_size;
} CartInfo;

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
  void flush_mmio_mappers();
  bool set_mmio_mapper(uint16 addr, MMIO *mapper);

private:
//0x1000000 / 512 = 32768
//512 = 0x200, smallest block of a different-speed memory range
//ex. $4000-$41ff = 512
uint8 *speed_table,
       speed_table_slowrom[32768],
       speed_table_fastrom[32768];
  inline uint8 calc_speed(uint32 addr, bool fast);
public:
  uint8 speed(uint32 addr);
  void  set_speed(bool fast);

  virtual bool load_cart(Reader *rf) = 0;
  virtual bool load_sram(Reader *rf) = 0;
  virtual bool save_sram(Writer *wf) = 0;
  virtual void unload_cart() = 0;
  virtual void get_cartinfo(CartInfo *ci) = 0;

  virtual void power() = 0;
  virtual void reset() = 0;

  MemBus();
  ~MemBus();
};
