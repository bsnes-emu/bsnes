#if defined(SFC_SUPERGAMEBOY)

struct ICD2 : Emulator::Interface::Bind, GameBoy::Interface::Hook, Cothread {
  static auto Enter() -> void;
  auto main() -> void;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  uint revision;

private:
  #include "interface/interface.hpp"
  #include "mmio/mmio.hpp"

  Emulator::Interface::Bind* bind = nullptr;
  GameBoy::Interface::Hook* hook = nullptr;
};

#else

struct ICD2 : Coprocessor {
  auto init() -> void {}
  auto load() -> void {}
  auto unload() -> void {}
  auto power() -> void {}
  auto reset() -> void {}

  auto read(uint24, uint8) -> uint8 { return 0; }
  auto write(uint24, uint8) -> void { return; }

  auto serialize(serializer&) -> void {}

  uint revision;
};

#endif

extern ICD2 icd2;
