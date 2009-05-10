struct Memory {
  virtual unsigned size() const { return 0; }
  virtual uint8 read(unsigned addr) = 0;
  virtual void write(unsigned addr, uint8 data) = 0;
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
  uint8* data() { return data_; }
  unsigned size() const { return size_; }

  inline uint8 read(unsigned addr) { return data_[addr]; }
  inline void write(unsigned addr, uint8 n) { data_[addr] = n; }
  inline uint8& operator[](unsigned addr) { return data_[addr]; }
  inline const uint8& operator[](unsigned addr) const { return data_[addr]; }

  StaticRAM(unsigned n) : size_(n) { data_ = new uint8[size_]; }
  ~StaticRAM() { delete[] data_; }

private:
  uint8 *data_;
  unsigned size_;
};

struct MappedRAM : Memory {
  void reset() {
    if(data_) {
      delete[] data_;
      data_ = 0;
    }
    size_ = -1U;
    write_protect_ = false;
  }

  void map(uint8 *source, unsigned length) {
    reset();
    data_ = source;
    size_ = data_ && length > 0 ? length : -1U;
  }

  void write_protect(bool status) { write_protect_ = status; }
  uint8* data() { return data_; }
  unsigned size() const { return size_; }

  inline uint8 read(unsigned addr) { return data_[addr]; }
  inline void write(unsigned addr, uint8 n) { if(!write_protect_) data_[addr] = n; }
  inline const uint8 operator[](unsigned addr) const { return data_[addr]; }
  MappedRAM() : data_(0), size_(-1U), write_protect_(false) {}

private:
  uint8 *data_;
  unsigned size_;
  bool write_protect_;
};

struct MMIOAccess : Memory {
  void map(unsigned addr, MMIO &access);
  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);

  MMIO *mmio[0x4000];
};

struct Bus {
  unsigned mirror(unsigned addr, unsigned size);
  void map(unsigned addr, Memory &access, unsigned offset);
  enum MapMode { MapDirect, MapLinear, MapShadow };
  void map(MapMode mode,
    uint8  bank_lo, uint8  bank_hi,
    uint16 addr_lo, uint16 addr_hi,
    Memory &access, unsigned offset = 0, unsigned size = 0);

  alwaysinline uint8 read(unsigned addr) {
    #if defined(CHEAT_SYSTEM)
    if(cheat.active() && cheat.exists(addr)) {
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

  virtual bool load_cart() { return false; }
  virtual void unload_cart() {}

  virtual void power() {}
  virtual void reset() {}

  Bus() {}
  virtual ~Bus() {}

  struct Page {
    Memory *access;
    unsigned offset;
  } page[65536];
};

namespace memory {
  extern MMIOAccess mmio;    //S-CPU, S-PPU
  extern StaticRAM  wram;    //S-CPU
  extern StaticRAM  apuram;  //S-SMP, S-DSP
  extern StaticRAM  vram;    //S-PPU
  extern StaticRAM  oam;     //S-PPU
  extern StaticRAM  cgram;   //S-PPU

  extern UnmappedMemory memory_unmapped;
  extern UnmappedMMIO   mmio_unmapped;
};
