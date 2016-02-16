struct NECDSP : Processor::uPD96050, Coprocessor {
  static auto Enter() -> void;
  auto main() -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  auto readRAM(uint24 addr, uint8 data) -> uint8;
  auto writeRAM(uint24 addr, uint8 data) -> void;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto firmware() const -> vector<uint8>;
  auto serialize(serializer&) -> void;
};

extern NECDSP necdsp;
