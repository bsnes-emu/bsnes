struct SharpRTC : Thread {
  static auto Enter() -> void;
  auto main() -> void;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto sync() -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  enum class State : uint { Ready, Command, Read, Write } state;
  int index;

  uint second;
  uint minute;
  uint hour;
  uint day;
  uint month;
  uint year;
  uint weekday;

  //memory.cpp
  auto rtcRead(uint4 addr) -> uint4;
  auto rtcWrite(uint4 addr, uint4 data) -> void;

  auto load(const uint8* data) -> void;
  auto save(uint8* data) -> void;

  //time.cpp
  static const uint daysInMonth[12];
  auto tickSecond() -> void;
  auto tickMinute() -> void;
  auto tickHour() -> void;
  auto tickDay() -> void;
  auto tickMonth() -> void;
  auto tickYear() -> void;

  auto calculateWeekday(uint year, uint month, uint day) -> uint;
};

extern SharpRTC sharprtc;
