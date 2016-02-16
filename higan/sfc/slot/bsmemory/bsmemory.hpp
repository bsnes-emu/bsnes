struct BSMemory : Memory {
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto size() const -> uint;
  auto read(uint24 addr, uint8) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  MappedRAM memory;
  bool readonly;

private:
  struct {
    uint command;
    uint8 write_old;
    uint8 write_new;

    bool flash_enable;
    bool read_enable;
    bool write_enable;
  } regs;
};

extern BSMemory bsmemory;
