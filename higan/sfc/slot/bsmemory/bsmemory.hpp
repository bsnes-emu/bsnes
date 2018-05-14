struct BSMemory : Memory {
  //bsmemory.cpp
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;

  auto size() const -> uint;
  auto read(uint24 addr, uint8) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint pathID = 0;
  MappedRAM memory;
  bool readonly;

private:
  struct {
    uint command;
    uint8 writeOld;
    uint8 writeNew;

    bool flashEnable;
    bool readEnable;
    bool writeEnable;
  } regs;
};

extern BSMemory bsmemory;
