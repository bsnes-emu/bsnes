//Motorola 68000

struct CPU : Processor::M68K, Processor::M68K::Bus, Thread {
  enum class Interrupt : uint {
    Reset,
    HorizontalBlank,
    VerticalBlank,
  };

  using Thread::synchronize;

  //cpu.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto synchronize() -> void;

  auto raise(Interrupt) -> void;
  auto lower(Interrupt) -> void;

  auto load(Markup::Node) -> bool;
  auto power(bool reset) -> void;

  //bus.cpp
  auto readByte(uint24 address) -> uint16 override;
  auto readWord(uint24 address) -> uint16 override;
  auto writeByte(uint24 address, uint16 data) -> void override;
  auto writeWord(uint24 address, uint16 data) -> void override;
  auto readIO(uint24 address) -> uint16;
  auto writeIO(uint24 address, uint16 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  vector<Thread*> peripherals;

private:
  uint8 ram[64 * 1024];
  uint8 tmss[2 * 1024];
  uint1 tmssEnable;

  struct IO {
    boolean version;  //0 = Model 1; 1 = Model 2+
    boolean romEnable;
    boolean vdpEnable[2];
  } io;

  struct State {
    uint32 interruptLine;
    uint32 interruptPending;
  } state;
};

extern CPU cpu;
