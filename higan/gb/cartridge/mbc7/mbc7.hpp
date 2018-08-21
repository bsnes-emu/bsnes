struct MBC7 : Mapper {
  enum : uint { Center = 0x81d0 };

  //mbc7.cpp
  auto load(Markup::Node document) -> void override;
  auto save(Markup::Node document) -> void override;
  auto main() -> void override;
  auto read(uint16 address) -> uint8 override;
  auto write(uint16 address, uint8 data) -> void override;
  auto power() -> void override;

  //serialization.cpp
  auto serialize(serializer&) -> void override;

  struct EEPROM {
    //eeprom.cpp
    auto load(Markup::Node document) -> void;
    auto save(Markup::Node document) -> void;
    auto main() -> void;
    auto power(bool reset = false) -> void;

    //Game Boy MBC7 interface
    auto readIO() -> uint8;
    auto writeIO(uint8 data) -> void;

    //chip commands
    auto read() -> void;
    auto write() -> void;
    auto erase() -> void;
    auto writeAll() -> void;
    auto eraseAll() -> void;
    auto writeEnable() -> void;
    auto writeDisable() -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    //it is awkward no matter if data is uint1[4096], uint8[512], or uint16[256]
    uint8 data[512];  //uint8 was chosen solely for easier serialization and saving
    uint size;        //in bits; not bytes
    uint width;       //8-bit (ORG=0) or 16-bit (ORG=1) configuration

    boolean select;    //CS
    boolean clock;     //CLK
    boolean writable;  //EWEN, EWDS
    uint busy;         //busy cycles in milliseconds remaining for programming (write) operations to complete

    struct ShiftRegister {
      auto bit(uint index) { return value.bit(index); }
      auto bits(uint lo, uint hi) { return value.bits(lo, hi); }
      auto byte(uint index) { return value.byte(index); }
      auto flush() -> void;
      auto peek() -> bool;
      auto read() -> bool;
      auto write(bool data) -> void;

      //serialization.cpp
      auto serialize(serializer&) -> void;

      uint32 value;
      uint32 count;
      uint32 length;
    } command, address, input, output;
  } eeprom;

  struct IO {
    struct ROM {
      uint8 bank = 0x01;
    } rom;
    struct RAM {
      uint1 enable[2];
    } ram;
    struct Accelerometer {
      uint16 x = Center;
      uint16 y = Center;
    } accelerometer;
  } io;
} mbc7;
