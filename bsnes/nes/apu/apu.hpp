struct APU : Processor {
  static void Main();
  void main();
  void tick();

  void power();
  void reset();

  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 data);

  struct Pulse {
    uint2 duty;
    bool halt;
    uint5 envelope;
    uint11 timer;
    uint5 length;
  } pulse1, pulse2;

  struct Triangle {
    bool control;
    uint7 reload;
    uint11 timer;
    uint5 length;
  } triangle;

  struct Noise {
    bool halt;
    uint5 envelope;
    bool mode;
    uint4 period;
    uint5 length;
  } noise;

  struct DMC {  //delta modulation channel
    bool irq;
    bool loop;
    uint4 rate;
    uint7 direct_load;
    uint16 sample_addr;
    uint12 sample_length;
  } dmc;
};

extern APU apu;
