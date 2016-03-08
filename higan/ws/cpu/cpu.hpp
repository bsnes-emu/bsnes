struct CPU : Processor::V30MZ, Thread, IO {
  enum class Interrupt : uint {
    SerialSend,
    Input,
    Cartridge,
    SerialReceive,
    LineCompare,
    VblankTimer,
    Vblank,
    HblankTimer,
  };

  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto wait(uint clocks = 1) -> void override;
  auto read(uint20 addr) -> uint8 override;
  auto write(uint20 addr, uint8 data) -> void override;
  auto in(uint16 port) -> uint8 override;
  auto out(uint16 port, uint8 data) -> void override;

  auto power() -> void;

  //io.cpp
  auto keypadRead() -> uint4;
  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;

  //interrupt.cpp
  auto poll() -> void;
  auto raise(Interrupt) -> void;
  auto lower(Interrupt) -> void;

  //dma.cpp
  auto dmaTransfer() -> void;

  struct Registers {
    //$0040-0042  DMA_SRC
    uint20 dmaSource;

    //$0044-0045  DMA_DST
    uint16 dmaTarget;

    //$0046-0047  DMA_LEN
    uint16 dmaLength;

    //$0048  DMA_CTRL
    uint1 dmaEnable;
    uint1 dmaMode;  //0 = increment; 1 = decrement

    //$00b0  INT_BASE
    uint8 interruptBase;

    //$00b2  INT_ENABLE
    uint8 interruptEnable;

    //$00b4  INT_STATUS
    uint8 interruptStatus;

    //$00b5  KEYPAD
    uint1 ypadEnable;
    uint1 xpadEnable;
    uint1 buttonEnable;
  } r;
};

extern CPU cpu;
