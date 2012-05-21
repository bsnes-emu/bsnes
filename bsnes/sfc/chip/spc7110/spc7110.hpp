struct SPC7110 : Coprocessor {
  unsigned prom_base, prom_size;  //program ROM
  unsigned drom_base, drom_size;  //data ROM

  uint4 rtcram[16];

  enum : unsigned {
    mul_delay =  6,
    div_delay =  8,
    rtc_delay = 20,
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

  //dcu.cpp
  void dcu_load_address();
  void dcu_begin_transfer();
  uint8 dcu_read();

  void decompress_1bpp(bool init = false);
  void decompress_2bpp(bool init = false);
  void decompress_4bpp(bool init = false);

  void deinterleave_1bpp(unsigned length);
  void deinterleave_2bpp(unsigned length);
  void deinterleave_4bpp(unsigned length);

  uint8 probability(unsigned n);
  uint8 next_lps(unsigned n);
  uint8 next_mps(unsigned n);
  bool toggle_invert(unsigned n);

  unsigned deinterleave_2x8(unsigned data);
  unsigned deinterleave_4x8(unsigned data);

  //data.cpp
  uint8 datarom_read(unsigned addr);

  unsigned data_offset();
  unsigned data_adjust();
  unsigned data_increment();

  void set_data_offset(unsigned addr);
  void set_data_adjust(unsigned addr);

  void data_port_read();

  void data_port_increment_a();
  void data_port_increment_b();
  void data_port_increment_c();

  //alu.cpp
  void alu_multiply();
  void alu_divide();

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
  uint8 r4807;  //deinterleave length
  uint8 r4809;  //compression length low
  uint8 r480a;  //compression length high
  uint8 r480b;  //deinterleave enable
  uint8 r480c;  //decompression status

  uint2 dcu_mode;
  uint23 dcu_addr;
  unsigned dcu_sp;
  unsigned dcu_dp;

  uint8 dcu_output[32];
  uint8 dcu_tiledata[256 * 32];

  struct ContextState {
    uint8 index;
    uint8 invert;
  } context[32];

  static const uint8 evolution_table[53][4];
  static const uint8 context_table[32][2];

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
};

extern SPC7110 spc7110;
