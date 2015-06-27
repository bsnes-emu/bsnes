struct CPUROM : Memory {
  auto size() const -> unsigned;
  auto read(unsigned) -> uint8;
  auto write(unsigned, uint8) -> void;
} cpurom;

struct CPURAM : Memory {
  auto size() const -> unsigned;
  auto read(unsigned) -> uint8;
  auto write(unsigned, uint8) -> void;
} cpuram;
