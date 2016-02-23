struct CPU : Processor::ARM, Thread, MMIO {
  using ARM::read;
  using ARM::write;

  struct Interrupt {
    enum : uint {
      VBlank       = 0x0001,
      HBlank       = 0x0002,
      VCoincidence = 0x0004,
      Timer0       = 0x0008,
      Timer1       = 0x0010,
      Timer2       = 0x0020,
      Timer3       = 0x0040,
      Serial       = 0x0080,
      DMA0         = 0x0100,
      DMA1         = 0x0200,
      DMA2         = 0x0400,
      DMA3         = 0x0800,
      Keypad       = 0x1000,
      Cartridge    = 0x2000,
    };
  };

  #include "registers.hpp"
  #include "prefetch.hpp"
  #include "state.hpp"

  //cpu.cpp
  CPU();
  ~CPU();

  static auto Enter() -> void;
  auto main() -> void;

  auto step(uint clocks) -> void override;

  auto sync_step(uint clocks) -> void;
  auto keypad_run() -> void;
  auto power() -> void;

  //bus.cpp
  auto bus_idle() -> void override;
  auto bus_read(uint mode, uint32 addr) -> uint32 override;
  auto bus_write(uint mode, uint32 addr, uint32 word) -> void override;
  auto bus_wait(uint mode, uint32 addr) -> uint;

  //mmio.cpp
  auto read(uint32 addr) -> uint8;
  auto write(uint32 addr, uint8 byte) -> void;

  auto iwram_read(uint mode, uint32 addr) -> uint32;
  auto iwram_write(uint mode, uint32 addr, uint32 word) -> void;

  auto ewram_read(uint mode, uint32 addr) -> uint32;
  auto ewram_write(uint mode, uint32 addr, uint32 word) -> void;

  //dma.cpp
  auto dma_run() -> void;
  auto dma_exec(Registers::DMA& dma) -> void;
  auto dma_vblank() -> void;
  auto dma_hblank() -> void;
  auto dma_hdma() -> void;

  //timer.cpp
  auto timer_step(uint clocks) -> void;
  auto timer_increment(uint n) -> void;
  auto timer_fifo_run(uint n) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint8* iwram = nullptr;
  uint8* ewram = nullptr;
};

extern CPU cpu;
