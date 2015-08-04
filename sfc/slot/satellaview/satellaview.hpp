struct SatellaviewCartridge : Memory {
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto size() const -> unsigned;
  auto read(unsigned addr) -> uint8;
  auto write(unsigned addr, uint8 data) -> void;

  MappedRAM memory;
  bool readonly;

private:
  struct {
    unsigned command;
    uint8 write_old;
    uint8 write_new;

    bool flash_enable;
    bool read_enable;
    bool write_enable;
  } regs;
};

extern SatellaviewCartridge satellaviewcartridge;
