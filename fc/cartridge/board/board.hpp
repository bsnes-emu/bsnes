struct Board {
  struct Memory {
    inline Memory(uint8_t* data, uint size) : data(data), size(size) {}
    inline Memory() : data(nullptr), size(0u), writable(false) {}
    inline ~Memory() { if(data) delete[] data; }

    inline auto read(uint addr) const -> uint8;
    inline auto write(uint addr, uint8 data) -> void;

    uint8_t* data;
    uint size;
    bool writable;
  };

  Board(Markup::Node& document);
  virtual ~Board() = default;

  static auto mirror(uint addr, uint size) -> uint;

  virtual auto main() -> void;
  virtual auto tick() -> void;

  virtual auto prg_read(uint addr) -> uint8 = 0;
  virtual auto prg_write(uint addr, uint8 data) -> void = 0;

  virtual auto chr_read(uint addr) -> uint8;
  virtual auto chr_write(uint addr, uint8 data) -> void;

  virtual inline auto scanline(uint y) -> void {}

  virtual auto power() -> void;
  virtual auto reset() -> void;

  virtual auto serialize(serializer&) -> void;

  static auto load(string manifest) -> Board*;

  struct Information {
    string type;
    bool battery;
  } information;

  Memory prgrom;
  Memory prgram;
  Memory chrrom;
  Memory chrram;
};
