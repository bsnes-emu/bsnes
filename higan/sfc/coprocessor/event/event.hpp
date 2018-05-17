//HLE of the NEC uPD78P214GC processor found on SNES-EVENT PCBs, used by:
//* Campus Challenge '92
//* PowerFest '94

//The NEC uPD78214 family are 8-bit microprocessors containing:
//* UART/CSI serial interface
//* ALU (MUL, DIV, BCD)
//* interrupts (12 internal; 7 external; 2 priority levels)
//* 16384 x 8-bit ROM
//*   512 x 8-bit RAM
//*     4 x timer/counters

//None of the SNES-EVENT games have had their uPD78214 firmware dumped.
//As such, our only option is very basic high-level emulation, provided here.

/* Unverified memory maps:

game
  label: Campus Challenge '92
  board
    memory type=RAM content=Save size=0x2000 volatile
      map address=70-7d,f0-ff:0000-7fff mask=0x8000
    processor manufacturer=NEC architecture=uPD78214
      map address=c0,e0:0000
      mcu
        map address=00-1f,80-9f:8000-ffff
        memory type=ROM content=Program size=0x40000 label: Menu
        memory type=ROM content=Level-1 size=0x80000 label: Super Mario World
        memory type=ROM content=Level-2 size=0x80000 label: F-Zero
        memory type=ROM content=Level-3 size=0x80000 label: Pilotwings
      dip switches=8
    processor manufacturer=NEC architecture=uPD7725 identifier=DSP1
      map address=20-3f,a0-bf:8000-ffff mask=0x7fff
      memory type=ROM content=Program size=0x1800 architecture=uPD7725
      memory type=ROM content=Data     size=0x800 architecture=uPD7725
      memory type=RAM content=Data     size=0x200 architecture=uPD7725 volatile
      oscillator frequency=7600000

game
  label: PowerFest '94
  board
    memory type=RAM content=Save size=0x2000 volatile
      map address=30-3f,b0-bf:6000-7fff mask=0xe000
    processor manufacturer=NEC architecture=uPD78214
      map address=10,20:6000
      mcu
        map address=00-3f,80-bf:8000-ffff
        map address=c0-ff:0000-ffff
        memory type=ROM content=Program  size=0x40000 label: Menu
        memory type=ROM content=Level-1  size=0x80000 label: Super Mario Bros.: The Lost Levels
        memory type=ROM content=Level-2  size=0x80000 label: Super Mario Kart
        memory type=ROM content=Level-3 size=0x100000 label: Ken Griffey Jr. Presents: Major League Baseball
      dip switches=8
    processor manufacturer=NEC architecture=uPD7725 identifier=DSP1
      map address=00-0f,80-8f:6000-7fff mask=0xfff
      memory type=ROM content=Program size=0x1800 architecture=uPD7725
      memory type=ROM content=Data     size=0x800 architecture=uPD7725
      memory type=RAM content=Data     size=0x200 architecture=uPD7725 volatile
      oscillator frequency=7600000
*/

struct Event : Thread {
  //event.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto unload() -> void;
  auto power() -> void;

  auto mcuRead(uint24 addr, uint8) -> uint8;
  auto mcuWrite(uint24 addr, uint8) -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

public:
  MappedRAM rom[4];

  enum class Board : uint { Unknown, CampusChallenge92, PowerFest94 } board;
  uint timer;

private:
  uint8 status;
  uint8 select;

  bool timerActive;
  bool scoreActive;

  uint timerSecondsRemaining;
  uint scoreSecondsRemaining;
};

extern Event event;
