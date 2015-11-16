struct {
  uint16 slot[8] = {0};
  uint32 addr = 0;  //read location of slot buffer
  uint32 load = 0;  //write location of slot buffer
  int wait = 0;     //number of clocks before next slot load

  auto empty() const { return addr == load; }
  auto full() const { return load - addr == 16; }
} prefetch;

auto prefetch_sync(uint32 addr) -> void;
auto prefetch_step(uint clocks) -> void;
auto prefetch_wait() -> void;
auto prefetch_read() -> uint16;
