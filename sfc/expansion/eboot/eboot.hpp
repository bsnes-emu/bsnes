struct eBoot : Memory {
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto read(uint addr) -> uint8;
  auto write(uint addr, uint8 data) -> void;

private:
  bool booted = false;
  uint16 resetVector = 0;
  uint8 ram[124] = {0};
};

extern eBoot eboot;
