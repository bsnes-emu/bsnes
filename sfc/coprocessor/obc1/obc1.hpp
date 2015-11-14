struct OBC1 {
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto read(uint addr) -> uint8;
  auto write(uint addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  MappedRAM ram;

private:
  auto ramRead(uint addr) -> uint8;
  auto ramWrite(uint addr, uint8 data) -> void;

  struct {
    uint16 address;
    uint16 baseptr;
    uint16 shift;
  } status;
};

extern OBC1 obc1;
