struct CPU : Processor::ARM, Thread, MMIO {
  using ARM::read;
  using ARM::write;

  uint8* iwram = nullptr;
  uint8* ewram = nullptr;
  #include "registers.hpp"
  #include "prefetch.hpp"
  #include "state.hpp"

  //cpu.cpp
  static auto Enter() -> void;

  auto main() -> void;

  auto step(unsigned clocks) -> void override;
  auto bus_idle(uint32 addr) -> void override;
  auto bus_read(uint32 addr, uint32 size, bool mode) -> uint32 override;
  auto bus_write(uint32 addr, uint32 size, bool mode, uint32 word) -> void override;

  auto sync_step(unsigned clocks) -> void;
  auto keypad_run() -> void;
  auto power() -> void;

  CPU();
  ~CPU();

  //mmio.cpp
  auto read(uint32 addr) -> uint8;
  auto write(uint32 addr, uint8 byte) -> void;

  auto iwram_read(uint32 addr, uint32 size) -> uint32;
  auto iwram_write(uint32 addr, uint32 size, uint32 word) -> void;

  auto ewram_read(uint32 addr, uint32 size) -> uint32;
  auto ewram_write(uint32 addr, uint32 size, uint32 word) -> void;

  //dma.cpp
  auto dma_run() -> void;
  auto dma_exec(Registers::DMA& dma) -> void;
  auto dma_vblank() -> void;
  auto dma_hblank() -> void;
  auto dma_hdma() -> void;

  //timer.cpp
  auto timer_step(unsigned clocks) -> void;
  auto timer_increment(unsigned n) -> void;
  auto timer_fifo_run(unsigned n) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;
};

extern CPU cpu;
