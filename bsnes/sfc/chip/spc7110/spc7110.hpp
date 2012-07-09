struct Decompressor;

struct SPC7110 : Coprocessor {
  MappedRAM prom;  //program ROM
  MappedRAM drom;  //data ROM
  MappedRAM ram;

  static void Enter();
  void enter();
  void init();
  void load();
  void unload();
  void power();
  void reset();

  void add_clocks(unsigned clocks);

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
  ~SPC7110();

  //dcu.cpp
  void dcu_load_address();
  void dcu_begin_transfer();
  uint8 dcu_read();

  void deinterleave_1bpp(unsigned length);
  void deinterleave_2bpp(unsigned length);
  void deinterleave_4bpp(unsigned length);

  //data.cpp
  uint8 datarom_read(unsigned addr);

  unsigned data_offset();
  unsigned data_adjust();
  unsigned data_stride();

  void set_data_offset(unsigned addr);
  void set_data_adjust(unsigned addr);

  void data_port_read();

  void data_port_increment_4810();
  void data_port_increment_4814();
  void data_port_increment_4815();
  void data_port_increment_481a();

  //alu.cpp
  void alu_multiply();
  void alu_divide();

private:
  //==================
  //decompression unit
  //==================
  uint8 r4801;  //compression table B0
  uint8 r4802;  //compression table B1
  uint7 r4803;  //compression table B2
  uint8 r4804;  //compression table index
  uint8 r4805;  //adjust length B0
  uint8 r4806;  //adjust length B1
  uint8 r4807;  //stride length
  uint8 r4809;  //compression counter B0
  uint8 r480a;  //compression counter B1
  uint8 r480b;  //decompression settings
  uint8 r480c;  //decompression status

  bool dcu_pending;
  uint2 dcu_mode;
  uint23 dcu_addr;
  unsigned dcu_offset;
  uint8 dcu_tile[32];
  Decompressor *decompressor;

  //==============
  //data port unit
  //==============
  uint8 r4810;  //data port read + seek
  uint8 r4811;  //data offset B0
  uint8 r4812;  //data offset B1
  uint7 r4813;  //data offset B2
  uint8 r4814;  //data adjust B0
  uint8 r4815;  //data adjust B1
  uint8 r4816;  //data stride B0
  uint8 r4817;  //data stride B1
  uint8 r4818;  //data port settings
  uint8 r481a;  //data port seek

  //=====================
  //arithmetic logic unit
  //=====================
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
  uint8 r482e;  //math settings
  uint8 r482f;  //math status

  bool mul_pending;
  bool div_pending;

  //===================
  //memory control unit
  //===================
  uint8 r4830;  //bank 0 mapping + SRAM write enable
  uint8 r4831;  //bank 1 mapping
  uint8 r4832;  //bank 2 mapping
  uint8 r4833;  //bank 3 mapping
  uint8 r4834;  //bank mapping settings
};

extern SPC7110 spc7110;
