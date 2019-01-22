struct CPU : Processor::TLCS900H, Thread {
  Emulator::Memory::Writable<uint8> ram;

  //cpu.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto power() -> void;

  auto setInterruptHblank(boolean line) -> void;
  auto setInterruptVblank(boolean line) -> void;

  //memory.cpp
  auto read(uint24 address) -> uint8 override;
  auto write(uint24 address, uint8 data) -> void override;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  struct IO {
    struct IRQ {
      boolean hblank;
      boolean vblank;
    } irq;
  } io;
};

extern CPU cpu;
