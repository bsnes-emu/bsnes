//Motorola 68000

struct CPU : Processor::M68K, Thread {
  enum class Interrupt : uint {
    Reset,
    HorizontalBlank,
    VerticalBlank,
  };

  using Thread::synchronize;

  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto synchronize() -> void;

  auto raise(Interrupt) -> void;
  auto lower(Interrupt) -> void;

  auto power() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  vector<Thread*> peripherals;

private:
  struct State {
    uint32 interruptLine;
    uint32 interruptPending;
  } state;
};

extern CPU cpu;
