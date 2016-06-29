struct CPU : Processor::ARM, Thread, IO {
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

  auto syncStep(uint clocks) -> void;
  auto keypadRun() -> void;
  auto power() -> void;

  //bus.cpp
  auto _idle() -> void override;
  auto _read(uint mode, uint32 addr) -> uint32 override;
  auto _write(uint mode, uint32 addr, uint32 word) -> void override;
  auto wait(uint mode, uint32 addr) -> uint;

  //io.cpp
  auto readIO(uint32 addr) -> uint8;
  auto writeIO(uint32 addr, uint8 byte) -> void;

  auto readIWRAM(uint mode, uint32 addr) -> uint32;
  auto writeIWRAM(uint mode, uint32 addr, uint32 word) -> void;

  auto readEWRAM(uint mode, uint32 addr) -> uint32;
  auto writeEWRAM(uint mode, uint32 addr, uint32 word) -> void;

  //dma.cpp
  auto dmaRun() -> void;
  auto dmaExecute(Registers::DMA& dma) -> void;
  auto dmaVblank() -> void;
  auto dmaHblank() -> void;
  auto dmaHDMA() -> void;

  //timer.cpp
  auto timerStep(uint clocks) -> void;
  auto timerIncrement(uint n) -> void;
  auto timerRunFIFO(uint n) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint8* iwram = nullptr;
  uint8* ewram = nullptr;
};

extern CPU cpu;
