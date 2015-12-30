struct CPUROM : Memory {
  auto size() const -> uint;
  auto read(uint, uint8) -> uint8;
  auto write(uint, uint8) -> void;
} cpurom;

struct CPURAM : Memory {
  auto size() const -> uint;
  auto read(uint, uint8) -> uint8;
  auto write(uint, uint8) -> void;
} cpuram;
