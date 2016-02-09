struct ICD2 : Emulator::Interface::Bind, GameBoy::Interface::Hook, Coprocessor {
  static auto Enter() -> void;
  auto main() -> void;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto read(uint addr, uint8 data) -> uint8;
  auto write(uint addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  uint revision;

private:
  #include "interface/interface.hpp"
  #include "mmio/mmio.hpp"

  Emulator::Interface::Bind* bind = nullptr;
  GameBoy::Interface::Hook* hook = nullptr;
};

extern ICD2 icd2;
