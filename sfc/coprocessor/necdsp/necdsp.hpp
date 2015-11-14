struct NECDSP : Processor::uPD96050, Coprocessor {
  static auto Enter() -> void;
  auto enter() -> void;

  auto read(uint addr) -> uint8;
  auto write(uint addr, uint8 data) -> void;

  auto ram_read(uint addr) -> uint8;
  auto ram_write(uint addr, uint8 data) -> void;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto firmware() const -> vector<uint8>;
  auto serialize(serializer&) -> void;

  unsigned Select;
};

extern NECDSP necdsp;
