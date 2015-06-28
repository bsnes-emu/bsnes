struct Prefetch {
  uint16 slot[8] = {0};
  uint3 input = 0;
  uint3 output = 0;

  bool stalled = true;
  unsigned slots = 0;
  signed wait = 0;
  uint32 addr = 0;
} prefetch;

auto prefetch_stall() -> void;
auto prefetch_start(uint32 addr) -> void;
auto prefetch_step(unsigned clocks) -> void;
auto prefetch_wait() -> void;
auto prefetch_take() -> uint16;
