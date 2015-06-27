struct Prefetch {
  struct Slot {
    uint32 addr;
    uint16 half;
  } slot[8];

  unsigned slots = 0;
  uint32 addr = 0;
} prefetch;

auto prefetch_run() -> void;
auto prefetch_read(uint32 addr, uint32 size) -> maybe<uint32>;
auto prefetch_take() -> uint16;
