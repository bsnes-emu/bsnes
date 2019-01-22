//K1GE (Neo Geo Pocket)
//K2GE (Neo Geo Pocket Color)

struct VPU : Thread {
  Emulator::Memory::Writable<uint8> ram;

  //vpu.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;
  auto power() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  uint32 buffer[160 * 152];

  struct IO {
    uint8  vcounter;
    uint10 hcounter;
  } io;
};

extern VPU vpu;
