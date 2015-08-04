struct Memory {
  virtual inline auto size() const -> unsigned;
  virtual auto read(unsigned addr) -> uint8 = 0;
  virtual auto write(unsigned addr, uint8 data) -> void = 0;
};

struct StaticRAM : Memory {
  inline StaticRAM(unsigned size);
  inline ~StaticRAM();

  inline auto data() -> uint8*;
  inline auto size() const -> unsigned;

  inline auto read(unsigned addr) -> uint8;
  inline auto write(unsigned addr, uint8 n) -> void;
  inline auto operator[](unsigned addr) -> uint8&;
  inline auto operator[](unsigned addr) const -> const uint8&;

private:
  uint8* data_ = nullptr;
  unsigned size_ = 0;
};

struct MappedRAM : Memory {
  inline auto reset() -> void;
  inline auto map(uint8*, unsigned) -> void;
  inline auto copy(const stream& memory) -> void;
  inline auto read(const stream& memory) -> void;

  inline auto write_protect(bool status) -> void;
  inline auto data() -> uint8*;
  inline auto size() const -> unsigned;

  inline auto read(unsigned addr) -> uint8;
  inline auto write(unsigned addr, uint8 n) -> void;
  inline auto operator[](unsigned addr) const -> const uint8&;

private:
  uint8* data_ = nullptr;
  unsigned size_ = 0;
  bool write_protect_ = false;
};

struct Bus {
  alwaysinline static auto mirror(unsigned addr, unsigned size) -> unsigned;
  alwaysinline static auto reduce(unsigned addr, unsigned mask) -> unsigned;

  Bus();
  ~Bus();

  alwaysinline auto read(unsigned addr) -> uint8;
  alwaysinline auto write(unsigned addr, uint8 data) -> void;

  auto reset() -> void;
  auto map() -> void;
  auto map(
    const function<uint8 (unsigned)>& reader,
    const function<void (unsigned, uint8)>& writer,
    unsigned banklo, unsigned bankhi,
    unsigned addrlo, unsigned addrhi,
    unsigned size = 0, unsigned base = 0, unsigned mask = 0
  ) -> void;

  uint8* lookup = nullptr;
  uint32* target = nullptr;

  unsigned idcount = 0;
  function<uint8 (unsigned)> reader[256];
  function<void (unsigned, uint8)> writer[256];
};

extern Bus bus;
