//Epson RTC-4513 Real-Time Clock

struct EpsonRTC : Coprocessor {
  static void Enter();
  void enter();

  void init();
  void load();
  void unload();
  void power();
  void reset();

  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);

  void serialize(serializer&);

  uint15 clocks;
  unsigned seconds;

  uint2 chipselect;
  enum class State : unsigned { Mode, Seek, Read, Write } state;
  uint4 mdr;
  uint4 offset;
  unsigned wait;
  bool ready;

  uint4 secondlo;
  uint3 secondhi;
  uint1 batteryfailure;

  uint4 minutelo;
  uint3 minutehi;
  uint1 minutecarry;

  uint4 hourlo;
  uint2 hourhi;
  uint1 meridian;
  uint1 hourcarry;

  uint4 daylo;
  uint2 dayhi;
  uint1 dayram;
  uint1 daycarry;

  uint4 monthlo;
  uint1 monthhi;
  uint2 monthram;
  uint1 monthcarry;

  uint4 yearlo;
  uint4 yearhi;

  uint3 weekday;
  uint1 weekdaycarry;

  uint1 hold;
  uint1 calendar;
  uint1 irqflag;
  uint1 roundseconds;

  uint1 irqmask;
  uint1 irqduty;
  uint2 irqperiod;

  uint1 pause;
  uint1 stop;
  uint1 atime;  //astronomical time (24-hour mode)
  uint1 test;

  //memory.cpp
  void rtc_reset();
  uint4 rtc_read(uint4 addr);
  void rtc_write(uint4 addr, uint4 data);

  void load(const uint8 *data);
  void save(uint8 *data);

  //time.cpp
  void irq(uint2 period);
  void duty();
  void tick();
  void tick_second();
  void tick_minute();
  void tick_hour();
  void tick_day();
  void tick_month();
  void tick_year();
};

extern EpsonRTC epsonrtc;
