struct CPU : Processor::LR35902, Thread, MMIO {
  enum class Interrupt : uint { Vblank, Stat, Timer, Serial, Joypad };

  static auto Main() -> void;
  auto main() -> void;
  auto interrupt_raise(Interrupt id) -> void;
  auto interrupt_test() -> void;
  auto interrupt_exec(uint16 pc) -> void;
  auto stop() -> bool;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  //mmio.cpp
  auto wram_addr(uint16 addr) const -> uint;
  auto mmio_joyp_poll() -> void;
  auto mmio_read(uint16 addr) -> uint8;
  auto mmio_write(uint16 addr, uint8 data) -> void;

  //memory.cpp
  auto op_io() -> void;
  auto op_read(uint16 addr) -> uint8;
  auto op_write(uint16 addr, uint8 data) -> void;
  auto cycle_edge() -> void;
  auto dma_read(uint16 addr) -> uint8;
  auto dma_write(uint16 addr, uint8 data) -> void;
  auto debugger_read(uint16 addr) -> uint8;

  //timing.cpp
  auto add_clocks(uint clocks) -> void;
  auto timer_262144hz() -> void;
  auto timer_65536hz() -> void;
  auto timer_16384hz() -> void;
  auto timer_8192hz() -> void;
  auto timer_4096hz() -> void;
  auto hblank() -> void;

  struct Status {
    uint22 clock;

    //$ff00  JOYP
    bool p15;
    bool p14;
    uint8 joyp;
    uint8 mlt_req;

    //$ff01  SB
    uint8 serial_data;
    uint serial_bits;

    //$ff02  SC
    bool serial_transfer;
    bool serial_clock;

    //$ff04  DIV
    uint16 div;

    //$ff05  TIMA
    uint8 tima;

    //$ff06  TMA
    uint8 tma;

    //$ff07  TAC
    bool timer_enable;
    uint timer_clock;

    //$ff0f  IF
    bool interrupt_request_joypad;
    bool interrupt_request_serial;
    bool interrupt_request_timer;
    bool interrupt_request_stat;
    bool interrupt_request_vblank;

    //$ff4d  KEY1
    bool speed_double;
    bool speed_switch;

    //$ff51,$ff52  HDMA1,HDMA2
    uint16 dma_source;

    //$ff53,$ff54  HDMA3,HDMA4
    uint16 dma_target;

    //$ff55  HDMA5
    bool dma_mode;
    uint16 dma_length;
    bool dma_completed;

    //$ff6c  ???
    uint8 ff6c;

    //$ff70  SVBK
    uint3 wram_bank;

    //$ff72-$ff75  ???
    uint8 ff72;
    uint8 ff73;
    uint8 ff74;
    uint8 ff75;

    //$ffff  IE
    bool interrupt_enable_joypad;
    bool interrupt_enable_serial;
    bool interrupt_enable_timer;
    bool interrupt_enable_stat;
    bool interrupt_enable_vblank;
  } status;

  struct OAMDMA {
    bool active;
    uint clock;
    uint8 bank;
  } oamdma;

  uint8 wram[32768];  //GB=8192, GBC=32768
  uint8 hram[128];
};

extern CPU cpu;
