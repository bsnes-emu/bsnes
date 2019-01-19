struct CPU : Processor::SM83, Thread, MMIO {
  enum class Interrupt : uint { Vblank, Stat, Timer, Serial, Joypad };

  static auto Enter() -> void;
  auto main() -> void;
  auto raise(Interrupt id) -> void;
  auto interruptTest() -> void;
  auto stop() -> bool;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  //io.cpp
  auto wramAddress(uint16 addr) const -> uint;
  auto joypPoll() -> void;
  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;

  //memory.cpp
  auto idle() -> void override;
  auto read(uint16 addr) -> uint8 override;
  auto write(uint16 addr, uint8 data) -> void override;
  auto cycleEdge() -> void;
  auto readDMA(uint16 addr) -> uint8;
  auto writeDMA(uint16 addr, uint8 data) -> void;
  auto readDebugger(uint16 addr) -> uint8 override;

  //timing.cpp
  auto step(uint clocks) -> void;
  auto timer262144hz() -> void;
  auto timer65536hz() -> void;
  auto timer16384hz() -> void;
  auto timer8192hz() -> void;
  auto timer4096hz() -> void;
  auto hblank() -> void;

  struct Status {
    uint22 clock;

    //$ff00  JOYP
    bool p15 = 0;
    bool p14 = 0;
    uint8 joyp;

    //$ff01  SB
    uint8 serialData;
    uint serialBits = 0;

    //$ff02  SC
    bool serialTransfer = 0;
    bool serialClock = 0;

    //$ff04  DIV
    uint16 div;

    //$ff05  TIMA
    uint8 tima;

    //$ff06  TMA
    uint8 tma;

    //$ff07  TAC
    bool timerEnable = 0;
    uint timerClock = 0;

    //$ff0f  IF
    bool interruptRequestJoypad = 0;
    bool interruptRequestSerial = 0;
    bool interruptRequestTimer = 0;
    bool interruptRequestStat = 0;
    bool interruptRequestVblank = 0;

    //$ff4d  KEY1
    bool speedDouble = 0;
    bool speedSwitch = 0;

    //$ff51,$ff52  HDMA1,HDMA2
    uint16 dmaSource;

    //$ff53,$ff54  HDMA3,HDMA4
    uint16 dmaTarget;

    //$ff55  HDMA5
    bool dmaMode = 0;
    uint16 dmaLength;
    bool dmaCompleted = 1;

    //$ff6c  ???
    uint8 ff6c;

    //$ff70  SVBK
    uint3 wramBank = 1;

    //$ff72-$ff75  ???
    uint8 ff72;
    uint8 ff73;
    uint8 ff74;
    uint8 ff75;

    //$ffff  IE
    bool interruptEnableJoypad = 0;
    bool interruptEnableSerial = 0;
    bool interruptEnableTimer = 0;
    bool interruptEnableStat = 0;
    bool interruptEnableVblank = 0;
  } status;

  uint8 wram[32768];  //GB=8192, GBC=32768
  uint8 hram[128];
};

extern CPU cpu;
