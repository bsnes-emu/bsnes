struct NSS {
  auto power() -> void;

  auto setDip(uint16 dip) -> void;
  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  uint8 dip = 0x00;
};

extern NSS nss;
