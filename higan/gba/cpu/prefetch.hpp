struct {
  uint16 slot[8] = {0};
  uint32 addr = 0;  //read location of slot buffer
  uint32 load = 0;  //write location of slot buffer
  int wait = 0;     //number of clocks before next slot load

  auto empty() const { return addr == load; }
  auto full() const { return load - addr == 16; }
} prefetch;

auto prefetchSync(uint32 addr) -> void;
auto prefetchStep(uint clocks) -> void;
auto prefetchWait() -> void;
auto prefetchRead() -> uint16;
