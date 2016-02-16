struct CPUROM : Memory {
  auto size() const -> uint;
  auto read(uint24, uint8) -> uint8;
  auto write(uint24, uint8) -> void;
} cpurom;

struct CPURAM : Memory {
  auto size() const -> uint;
  auto read(uint24, uint8) -> uint8;
  auto write(uint24, uint8) -> void;
} cpuram;
