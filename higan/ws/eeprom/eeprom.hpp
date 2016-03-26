//93LC46 (  64x16-bit) [ 128 bytes]
//93LC56 ( 128x16-bit) [ 256 bytes]
//93LC66 ( 256x16-bit) [ 512 bytes]
//93LC76 ( 512x16-bit) [1024 bytes]
//93LC86 (1024x16-bit) [2048 bytes]

struct EEPROM {
  enum : uint {
    DataLo,
    DataHi,
    AddressLo,
    AddressHi,
    Status,
    Command = Status,
  };

  auto name() const -> string;
  auto data() -> uint16*;
  auto size() const -> uint;

  auto setName(string name) -> void;
  auto setSize(uint size) -> void;

  auto erase() -> void;
  auto power() -> void;
  auto read(uint) -> uint8;
  auto write(uint, uint8) -> void;

  auto operator[](uint10 addr) -> uint16&;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  auto execute() -> void;

  string _name;
  uint16 _data[1024];
  uint _size = 0;  //in words

  struct Registers {
    uint16 latch;
    uint16 address;

    bool unknown;
    bool writeRequested;
    bool readRequested;

    bool writeCompleted;
    bool readCompleted;

    bool writeProtect;
  } r;
};
