struct SuperDisc : Coprocessor, Memory {
  static auto Enter() -> void;
  auto main() -> void;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  //nec.cpp
  auto necPollIRQ() -> uint8;
  auto necReadData() -> uint8;
  auto necWriteCommand(uint4 data) -> void;

  //sony.cpp
  auto sonyPollIRQ() -> uint8;
  auto sonyReadData() -> uint8;
  auto sonyWriteCommand(uint8 data) -> void;
  auto sonyWriteData(uint8 data) -> void;

private:
  struct Registers {
    uint8 irqEnable;
  } r;

  //NEC
  struct NEC {
    vector<uint4> command;
    uint8 data;
  } nec;

  //Sony
  struct Sony {
    uint8 command;
    uint8 data;
  } sony;
};

extern SuperDisc superdisc;
