struct Board {
  struct Memory {
    inline Memory(uint8_t* data, uint size) : data(data), size(size) {}
    inline Memory() : data(nullptr), size(0u), writable(false) {}
    inline ~Memory() { if(data) delete[] data; }

    inline auto read(uint addr) const -> uint8;
    inline auto write(uint addr, uint8 data) -> void;

    string name;
    uint8_t* data = nullptr;
    uint size = 0;
    bool writable = false;
  };

  virtual ~Board() = default;

  static auto mirror(uint addr, uint size) -> uint;

  Board(Markup::Node& document);
  auto save() -> void;

  virtual auto main() -> void;
  virtual auto tick() -> void;

  virtual auto readPRG(uint addr) -> uint8 = 0;
  virtual auto writePRG(uint addr, uint8 data) -> void = 0;

  virtual auto readCHR(uint addr) -> uint8;
  virtual auto writeCHR(uint addr, uint8 data) -> void;

  virtual inline auto scanline(uint y) -> void {}

  virtual auto power() -> void;

  virtual auto serialize(serializer&) -> void;

  static auto load(string manifest) -> Board*;

  struct Information {
    string type;
  } information;

  Memory prgrom;
  Memory prgram;
  Memory chrrom;
  Memory chrram;
};
