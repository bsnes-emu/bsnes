struct DIP {
  //dip.cpp
  auto power() -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint8 value = 0x00;
};

extern DIP dip;
