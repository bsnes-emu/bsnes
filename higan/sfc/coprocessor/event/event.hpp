//SNES-EVENT board emulation:
//* Campus Challenge '92
//* Powerfest '94

struct Event : Coprocessor {
  static auto Enter() -> void;
  auto main() -> void;
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto mcuRead(uint addr, uint8) -> uint8;
  auto mcuWrite(uint addr, uint8) -> void;

  auto read(uint addr, uint8 data) -> uint8;
  auto write(uint addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  MappedRAM rom[4];
  MappedRAM ram;

  enum class Board : uint { CampusChallenge92, Powerfest94 } board;
  uint timer;

privileged:
  uint8 status;
  uint8 select;

  bool timerActive;
  bool scoreActive;

  uint timerSecondsRemaining;
  uint scoreSecondsRemaining;
};

extern Event event;
