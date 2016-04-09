struct Memory {
  virtual inline auto size() const -> uint;
  virtual auto read(uint24 addr, uint8 data = 0) -> uint8 = 0;
  virtual auto write(uint24 addr, uint8 data) -> void = 0;
};

struct StaticRAM : Memory {
  inline StaticRAM(uint size);
  inline ~StaticRAM();

  inline auto data() -> uint8*;
  inline auto size() const -> uint;

  inline auto read(uint24 addr, uint8 data = 0) -> uint8;
  inline auto write(uint24 addr, uint8 data) -> void;
  inline auto operator[](uint24 addr) -> uint8&;
  inline auto operator[](uint24 addr) const -> const uint8&;

private:
  uint8* data_ = nullptr;
  uint size_ = 0;
};

struct MappedRAM : Memory {
  inline auto reset() -> void;
  inline auto map(uint8*, uint) -> void;
  inline auto copy(const stream& memory) -> void;
  inline auto read(const stream& memory) -> void;

  inline auto write_protect(bool status) -> void;
  inline auto data() -> uint8*;
  inline auto size() const -> uint;

  inline auto read(uint24 addr, uint8 data = 0) -> uint8;
  inline auto write(uint24 addr, uint8 data) -> void;
  inline auto operator[](uint24 addr) const -> const uint8&;

private:
  uint8* data_ = nullptr;
  uint size_ = 0;
  bool write_protect_ = false;
};

struct Bus {
  alwaysinline static auto mirror(uint addr, uint size) -> uint;
  alwaysinline static auto reduce(uint addr, uint mask) -> uint;

  ~Bus();

  alwaysinline auto read(uint24 addr, uint8 data) -> uint8;
  alwaysinline auto write(uint24 addr, uint8 data) -> void;

  auto reset() -> void;
  auto map(
    const function<uint8 (uint24, uint8)>& read,
    const function<void (uint24, uint8)>& write,
    const string& addr, uint size = 0, uint base = 0, uint mask = 0
  ) -> void;
  auto unmap(const string& addr) -> void;

private:
  uint8* lookup = nullptr;
  uint32* target = nullptr;

  function<auto (uint24, uint8) -> uint8> reader[256];
  function<auto (uint24, uint8) -> void> writer[256];
  uint24 counter[256];
};

extern Bus bus;
