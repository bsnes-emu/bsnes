struct CPUIRAM : Memory {
  auto size() const -> uint;
  alwaysinline auto read(uint24, uint8 = 0) -> uint8;
  alwaysinline auto write(uint24, uint8) -> void;
} cpuiram;

struct CPUBWRAM : Memory {
  auto size() const -> uint;
  alwaysinline auto read(uint24, uint8 = 0) -> uint8;
  alwaysinline auto write(uint24, uint8) -> void;
  bool dma;
} cpubwram;
