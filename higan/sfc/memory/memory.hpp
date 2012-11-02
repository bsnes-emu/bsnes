struct Memory {
  virtual inline unsigned size() const;
  virtual uint8 read(unsigned addr) = 0;
  virtual void write(unsigned addr, uint8 data) = 0;
};

struct StaticRAM : Memory {
  inline uint8* data();
  inline unsigned size() const;

  inline uint8 read(unsigned addr);
  inline void write(unsigned addr, uint8 n);
  inline uint8& operator[](unsigned addr);
  inline const uint8& operator[](unsigned addr) const;

  inline StaticRAM(unsigned size);
  inline ~StaticRAM();

private:
  uint8 *data_;
  unsigned size_;
};

struct MappedRAM : Memory {
  inline void reset();
  inline void map(uint8*, unsigned);
  inline void copy(const stream &memory);
  inline void read(const stream &memory);

  inline void write_protect(bool status);
  inline uint8* data();
  inline unsigned size() const;

  inline uint8 read(unsigned addr);
  inline void write(unsigned addr, uint8 n);
  inline const uint8& operator[](unsigned addr) const;
  inline MappedRAM();

private:
  uint8 *data_;
  unsigned size_;
  bool write_protect_;
};

struct Bus {
  alwaysinline static unsigned mirror(unsigned addr, unsigned size);
  alwaysinline static unsigned reduce(unsigned addr, unsigned mask);

  alwaysinline uint8 read(unsigned addr);
  alwaysinline void write(unsigned addr, uint8 data);

  uint8 *lookup;
  uint32 *target;

  unsigned idcount;
  function<uint8 (unsigned)> reader[256];
  function<void (unsigned, uint8)> writer[256];

  void map(
    const function<uint8 (unsigned)> &reader,
    const function<void (unsigned, uint8)> &writer,
    unsigned banklo, unsigned bankhi,
    unsigned addrlo, unsigned addrhi,
    unsigned size = 0, unsigned base = 0, unsigned mask = 0
  );

  void map_reset();
  void map_xml();

  Bus();
  ~Bus();
};

extern Bus bus;
