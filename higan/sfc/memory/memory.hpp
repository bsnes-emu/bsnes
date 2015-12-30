struct Memory {
  virtual inline auto size() const -> uint;
  virtual auto read(uint addr, uint8 data = 0) -> uint8 = 0;
  virtual auto write(uint addr, uint8 data) -> void = 0;
};

struct StaticRAM : Memory {
  inline StaticRAM(uint size);
  inline ~StaticRAM();

  inline auto data() -> uint8*;
  inline auto size() const -> uint;

  inline auto read(uint addr, uint8 data = 0) -> uint8;
  inline auto write(uint addr, uint8 data) -> void;
  inline auto operator[](uint addr) -> uint8&;
  inline auto operator[](uint addr) const -> const uint8&;

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

  inline auto read(uint addr, uint8 data = 0) -> uint8;
  inline auto write(uint addr, uint8 data) -> void;
  inline auto operator[](uint addr) const -> const uint8&;

private:
  uint8* data_ = nullptr;
  uint size_ = 0;
  bool write_protect_ = false;
};

struct Bus {
  alwaysinline static auto mirror(uint addr, uint size) -> uint;
  alwaysinline static auto reduce(uint addr, uint mask) -> uint;

  Bus();
  ~Bus();

  alwaysinline auto read(uint addr, uint8 data) -> uint8;
  alwaysinline auto write(uint addr, uint8 data) -> void;

  auto reset() -> void;
  auto map() -> void;
  auto map(
    const function<uint8 (uint, uint8)>& reader,
    const function<void (uint, uint8)>& writer,
    uint banklo, uint bankhi, uint addrlo, uint addrhi,
    uint size = 0, uint base = 0, uint mask = 0
  ) -> void;

  uint8* lookup = nullptr;
  uint32* target = nullptr;

  uint idcount = 0;
  function<auto (uint, uint8) -> uint8> reader[256];
  function<auto (uint, uint8) -> void> writer[256];
};

extern Bus bus;
