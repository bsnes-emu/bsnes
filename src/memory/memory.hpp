struct Memory {
  virtual unsigned size() { return 0; }
  virtual uint8 read(unsigned addr) = 0;
  virtual void write(unsigned addr, uint8 data) = 0;

  //deprecated, still used by S-CPU, S-SMP disassemblers
  enum { WRAP_NONE = 0, WRAP_BANK = 1, WRAP_PAGE = 2 };
  virtual uint16 read_word(unsigned addr, unsigned wrap = WRAP_NONE);
  virtual void write_word(unsigned addr, uint16 data, unsigned wrap = WRAP_NONE);
  virtual uint32 read_long(unsigned addr, unsigned wrap = WRAP_NONE);
  virtual void write_long(unsigned addr, uint32 data, unsigned wrap = WRAP_NONE);
};

struct MMIO {
  virtual uint8 mmio_read(unsigned addr) = 0;
  virtual void mmio_write(unsigned addr, uint8 data) = 0;
};

struct UnmappedMemory : Memory {
  uint8 read(unsigned);
  void write(unsigned, uint8);
};

struct UnmappedMMIO : MMIO {
  uint8 mmio_read(unsigned);
  void mmio_write(unsigned, uint8);
};

struct StaticRAM : Memory {
  uint8* handle() { return data; }
  unsigned size() { return datasize; }

  inline uint8 read(unsigned addr) { return data[addr]; }
  inline void write(unsigned addr, uint8 n) { data[addr] = n; }
  inline uint8& operator[](unsigned addr) { return data[addr]; }
  inline const uint8& operator[](unsigned addr) const { return data[addr]; }

  StaticRAM(unsigned n) : datasize(n) { data = new uint8[datasize]; }
  ~StaticRAM() { delete[] data; }

private:
  uint8 *data;
  unsigned datasize;
};

struct MappedRAM : Memory {
  void map(uint8 *source, unsigned length) { data = source; datasize = length > 0 ? length : -1U; }
  void write_protect(bool status) { write_protection = status; }
  uint8* handle() { return data; }
  unsigned size() { return datasize; }

  inline uint8 read(unsigned addr) { return data[addr]; }
  inline void write(unsigned addr, uint8 n) { if(!write_protection) data[addr] = n; }
  inline const uint8& operator[](unsigned addr) const { return data[addr]; }

  MappedRAM() : data(0), datasize(0), write_protection(false) {}

private:
  uint8 *data;
  unsigned datasize;
  bool write_protection;
};

struct MMIOAccess : Memory {
  void map(unsigned addr, MMIO &access);
  MMIO* get(unsigned addr);
  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);

private:
  MMIO *mmio[0x4000];
};

class Bus {
public:
  unsigned mirror(unsigned addr, unsigned size);
  void map(unsigned addr, Memory &access, unsigned offset);
  enum MapMode { MapDirect, MapLinear, MapShadow };
  void map(MapMode mode,
    uint8  bank_lo, uint8  bank_hi,
    uint16 addr_lo, uint16 addr_hi,
    Memory &access, unsigned offset = 0, unsigned size = 0);

  alwaysinline uint8 read(unsigned addr) {
    #if defined(CHEAT_SYSTEM)
    if(cheat.enabled() && cheat.exists(addr)) {
      uint8 r;
      if(cheat.read(addr, r)) return r;
    }
    #endif

    Page &p = page[addr >> 8];
    return p.access->read(p.offset + addr);
  }

  alwaysinline void write(unsigned addr, uint8 data) {
    Page &p = page[addr >> 8];
    return p.access->write(p.offset + addr, data);
  }

  void set_speed(bool fast) {
    fastSpeed = fast ? 6 : 8;
  }

  alwaysinline unsigned speed(unsigned addr) const {
    if(addr & 0x408000) {
      if(addr & 0x800000) return fastSpeed;
      return 8;
    }
    if((addr + 0x6000) & 0x4000) return 8;
    if((addr - 0x4000) & 0x7e00) return 6;
    return 12;
  }

  virtual void load_cart() = 0;
  virtual void unload_cart() = 0;
  virtual bool cart_loaded() = 0;

  virtual void power() = 0;
  virtual void reset() = 0;

  Bus() {}
  virtual ~Bus() {}

protected:
  struct Page {
    Memory *access;
    unsigned offset;
  } page[65536];
  unsigned fastSpeed;
};

namespace memory {
  extern MMIOAccess mmio;   //S-CPU, S-PPU
  extern StaticRAM  wram;   //S-CPU
  extern StaticRAM  apuram; //S-SMP, S-DSP
  extern StaticRAM  vram;   //S-PPU
  extern StaticRAM  oam;    //S-PPU
  extern StaticRAM  cgram;  //S-PPU

  extern UnmappedMemory memory_unmapped;
  extern UnmappedMMIO   mmio_unmapped;
};
