struct Cartridge : Thread, IO {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;

  auto load() -> void;
  auto unload() -> void;

  //memory.cpp
  auto romRead(uint20 addr) -> uint8;
  auto romWrite(uint20 addr, uint8 data) -> void;

  auto ramRead(uint20 addr) -> uint8;
  auto ramWrite(uint20 addr, uint8 data) -> void;

  //rtc.cpp
  auto rtcLoad() -> void;
  auto rtcSave() -> void;
  auto rtcTickSecond() -> void;
  auto rtcCheckAlarm() -> void;
  auto rtcStatus() -> uint8;
  auto rtcCommand(uint8 data) -> void;
  auto rtcRead() -> uint8;
  auto rtcWrite(uint8 data) -> void;

  //io.cpp
  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct Information {
    string manifest;
    string title;
    bool orientation;  //0 = horizontal; 1 = vertical
    string sha256;
  } information;

  struct Registers {
    //$00c0  BANK_ROM2
    uint8 romBank2;

    //$00c1  BANK_SRAM
    uint8 sramBank;

    //$00c2  BANK_ROM0
    uint8 romBank0;

    //$00c3  BANK_ROM1
    uint8 romBank1;

    //$00cc  GPO_EN
    uint8 gpoEnable;

    //$00cd  GPO_DATA
    uint8 gpoData;
  } r;

  struct Memory {
    uint8* data = nullptr;
    uint size = 0;
    uint mask = 0;
    string name;
  };

  struct RTC : Memory {
    uint8 command;
    uint4 index;

    uint8 alarm;
    uint8 alarmHour;
    uint8 alarmMinute;

    auto year()    -> uint8& { return data[0]; }
    auto month()   -> uint8& { return data[1]; }
    auto day()     -> uint8& { return data[2]; }
    auto weekday() -> uint8& { return data[3]; }
    auto hour()    -> uint8& { return data[4]; }
    auto minute()  -> uint8& { return data[5]; }
    auto second()  -> uint8& { return data[6]; }
  };

  Memory rom;
  Memory ram;
  EEPROM eeprom;
  RTC rtc;
};

extern Cartridge cartridge;
