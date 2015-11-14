//SNES-EVENT board emulation:
//* Campus Challenge '92
//* Powerfest '94

struct Event : Coprocessor {
  static auto Enter() -> void;
  auto enter() -> void;
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto submitScore() -> void;

  auto sr(uint) -> uint8;
  auto dr(uint, uint8 data) -> void;
  auto rom_read(uint addr) -> uint8;
  auto ram_read(uint addr) -> uint8;
  auto ram_write(uint addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  MappedRAM rom[4];
  MappedRAM ram;

  enum class Board : uint { CampusChallenge92, Powerfest94 } board;
  uint revision;
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
