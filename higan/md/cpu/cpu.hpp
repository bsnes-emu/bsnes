//Motorola 68000

struct CPU : Processor::M68K, Thread {
  enum class Interrupt : uint {
    HorizontalBlank,
    VerticalBlank,
  };

  using Thread::synchronize;

  static auto Enter() -> void;
  auto boot() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto synchronize() -> void;

  auto raise(Interrupt) -> void;
  auto lower(Interrupt) -> void;

  auto power() -> void;
  auto reset() -> void;

  vector<Thread*> peripherals;

private:
  struct State {
    uint32 interruptLine;
    uint32 interruptPending;
  } state;
};

extern CPU cpu;
