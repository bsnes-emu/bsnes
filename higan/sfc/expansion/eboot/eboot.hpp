struct eBoot : Memory {
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

private:
  bool booted = false;
  uint16 resetVector;
  uint8 ram[124];
};

extern eBoot eboot;
