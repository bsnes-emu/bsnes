struct SharpRTC : Coprocessor {
  static void Enter();
  void enter();

  void init();
  void load();
  void unload();
  void power();
  void reset();
  void sync();

  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);

  void serialize(serializer&);

  enum class State : unsigned { Ready, Command, Read, Write } rtc_state;
  signed rtc_index;

  unsigned second;
  unsigned minute;
  unsigned hour;
  unsigned day;
  unsigned month;
  unsigned year;
  unsigned weekday;

  //memory.cpp
  uint4 rtc_read(uint4 addr);
  void rtc_write(uint4 addr, uint4 data);

  void load(const uint8* data);
  void save(uint8* data);

  //time.cpp
  static const unsigned daysinmonth[12];
  void tick_second();
  void tick_minute();
  void tick_hour();
  void tick_day();
  void tick_month();
  void tick_year();

  unsigned calculate_weekday(unsigned year, unsigned month, unsigned day);
};

extern SharpRTC sharprtc;
