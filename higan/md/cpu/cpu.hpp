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

  auto readByte(uint24 addr) -> uint8 override;
  auto readWord(uint24 addr) -> uint16 override;
  auto writeByte(uint24 addr, uint8 data) -> void override;
  auto writeWord(uint24 addr, uint16 data) -> void override;

private:
  uint8 ram[64 * 1024];

  struct State {
    uint32 interruptLine;
    uint32 interruptPending;
  } state;
};

extern CPU cpu;
