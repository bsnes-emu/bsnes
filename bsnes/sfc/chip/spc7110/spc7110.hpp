struct SPC7110 : Coprocessor {
  unsigned prom_base, prom_size;  //program ROM
  unsigned drom_base, drom_size;  //data ROM
  uint4 rtcram[16];

  enum : unsigned {
    mul_delay =  6,
    div_delay =  8,
    rtc_delay = 24,
  };

  static void Enter();
  void enter();
  void init();
  void load();
  void unload();
  void power();
  void reset();

  uint8 mmio_read(unsigned addr);
  void mmio_write(unsigned addr, uint8 data);

  uint8 dcu_read(unsigned);
  void dcu_write(unsigned, uint8);

  uint8 mcurom_read(unsigned addr);
  void mcurom_write(unsigned addr, uint8 data);

  uint8 mcuram_read(unsigned addr);
  void mcuram_write(unsigned addr, uint8 data);

  void serialize(serializer&);
  SPC7110();

  //data.cpp
  uint8 datarom_read(unsigned addr);

  unsigned data_offset();
  unsigned data_adjust();
  unsigned data_increment();

  void set_data_offset(unsigned addr);
  void set_data_adjust(unsigned addr);

  void data_port_read_a();
  void data_port_read_b();
  void data_port_read();

  void data_port_increment_a();
  void data_port_increment_b();
  void data_port_increment();

  //alu.cpp
  void alu_multiply();
  void alu_divide();

  //rtc.cpp
  void rtc_reset();
  void rtc_duty();
  void rtc_irq(uint2 frequency);
  uint4 rtc_read(uint4 addr);
  void rtc_write(uint4 addr, uint4 data);

  void rtc_pulse();
  void rtc_second();
  void rtc_minute();
  void rtc_hour();
  void rtc_day();
  void rtc_month();
  void rtc_year();

  void rtcram_load(const uint8 *data);
  void rtcram_save(uint8 *data);

private:
  //==================
  //decompression unit
  //==================
  uint8 r4801;  //compression table low
  uint8 r4802;  //compression table high
  uint8 r4803;  //compression table bank
  uint8 r4804;  //compression table index
  uint8 r4805;  //decompression buffer index low
  uint8 r4806;  //decompression buffer index high
  uint8 r4807;  //??? (used when $480b.d0 = 1)
  uint8 r4808;  //???
  uint8 r4809;  //compression length low
  uint8 r480a;  //compression length high
  uint8 r480b;  //decompression control register
  uint8 r480c;  //decompression status

  #include "decomp.hpp"
  Decomp decomp;

  //==============
  //data port unit
  //==============
  uint8 r4810;  //data port A
  uint8 r4811;  //data offset low
  uint8 r4812;  //data offset high
  uint8 r4813;  //data offset bank
  uint8 r4814;  //data adjust low
  uint8 r4815;  //data adjust high
  uint8 r4816;  //data increment low
  uint8 r4817;  //data increment high
  uint8 r4818;  //data port control register
  uint8 r481a;  //data port B

  bool r4814_latch;
  bool r4815_latch;

  //=========
  //math unit
  //=========
  uint8 r4820;  //16-bit multiplicand B0, 32-bit dividend B0
  uint8 r4821;  //16-bit multiplicand B1, 32-bit dividend B1
  uint8 r4822;  //32-bit dividend B2
  uint8 r4823;  //32-bit dividend B3
  uint8 r4824;  //16-bit multiplier B0
  uint8 r4825;  //16-bit multiplier B1
  uint8 r4826;  //16-bit divisor B0
  uint8 r4827;  //16-bit divisor B1
  uint8 r4828;  //32-bit product B0, 32-bit quotient B0
  uint8 r4829;  //32-bit product B1, 32-bit quotient B1
  uint8 r482a;  //32-bit product B2, 32-bit quotient B2
  uint8 r482b;  //32-bit product B3, 32-bit quotient B3
  uint8 r482c;  //16-bit remainder B0
  uint8 r482d;  //16-bit remainder B1
  uint8 r482e;  //math sign extend mode
  uint8 r482f;  //math status

  unsigned mul_wait;
  unsigned div_wait;

  //===================
  //memory mapping unit
  //===================
  uint8 r4830;  //bank 0 mapping + SRAM write enable
  uint8 r4831;  //bank 1 mapping
  uint8 r4832;  //bank 2 mapping
  uint8 r4833;  //bank 3 mapping
  uint8 r4834;  //bank mapping control

  //====================
  //real-time clock unit
  //====================
  uint8 r4840;  //RTC enable
  uint8 r4841;  //RTC data port
  uint8 r4842;  //RTC status

  uint22 rtc_clocks;
  unsigned rtc_seconds;
  uint2 rtc_mode;  //0 = idle, 1 = seek, 2 = read, 3 = write
  uint4 rtc_addr;
  unsigned rtc_wait;
};

extern SPC7110 spc7110;
