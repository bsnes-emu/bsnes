//HuC6260 -- Video Color Encoder

struct VCE : Thread {
  inline auto clock() const -> uint { return io.clock; }

  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;
  auto power() -> void;

  //io.cpp
  auto read(uint3 addr) -> uint8;
  auto write(uint3 addr, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  uint32 buffer[1365 * 263];

  struct CRAM {
    //memory.cpp
    auto read(uint9 addr) -> uint9;
    auto write(uint9 addr, bool a0, uint8 data) -> void;

    uint9 data[0x200];
    uint9 address;
  } cram;

  struct Timing {
    uint hclock;
    uint vclock;
  } timing;

  struct IO {
    uint clock;
    bool extraLine;
    bool grayscale;
  } io;
};

extern VCE vce;
