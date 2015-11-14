struct CPUIRAM : Memory {
  auto size() const -> uint;
  alwaysinline auto read(uint) -> uint8;
  alwaysinline auto write(uint, uint8) -> void;
} cpuiram;

struct CPUBWRAM : Memory {
  auto size() const -> uint;
  alwaysinline auto read(uint) -> uint8;
  alwaysinline auto write(uint, uint8) -> void;
  bool dma;
} cpubwram;
