enum class Input : uint {
  A, B, Select, Start, Right, Left, Up, Down, R, L,
};

struct BIOS : Memory {
  BIOS();
  ~BIOS();

  auto read(uint mode, uint32 addr) -> uint32 override;
  auto write(uint mode, uint32 addr, uint32 word) -> void override;

  uint8* data = nullptr;
  uint size = 0;
  uint32 mdr = 0;
};

struct System {
  auto init() -> void;
  auto term() -> void;
  auto load() -> void;
  auto power() -> void;
  auto run() -> void;
  auto runtosave() -> void;
  auto runthreadtosave() -> void;

  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto serialize(serializer&) -> void;
  auto serialize_all(serializer&) -> void;
  auto serialize_init() -> void;

  struct Information {
    string manifest;
  } information;

  uint serialize_size;
};

extern BIOS bios;
extern System system;
