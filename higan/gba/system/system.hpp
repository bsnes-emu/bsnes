enum class Input : uint {
  A, B, Select, Start, Right, Left, Up, Down, R, L,
};

struct BIOS {
  BIOS();
  ~BIOS();

  auto read(uint mode, uint32 addr) -> uint32;
  auto write(uint mode, uint32 addr, uint32 word) -> void;

  uint8* data = nullptr;
  uint size = 0;
  uint32 mdr = 0;
};

struct System {
  auto loaded() const -> bool { return _loaded; }

  auto init() -> void;
  auto term() -> void;
  auto load(Emulator::Interface*) -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto run() -> void;
  auto runToSave() -> void;

  //video.cpp
  auto configureVideoPalette() -> void;
  auto configureVideoEffects() -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&) -> void;
  auto serializeInit() -> void;

private:
  Emulator::Interface* interface = nullptr;

  struct Information {
    string manifest;
  } information;

  bool _loaded = false;
  uint _serializeSize = 0;
};

extern BIOS bios;
extern System system;
