struct APU : Processor {
  static void Main();
  void main();
  void tick();

  void power();
  void reset();

  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 data);

  APU();

  struct Envelope {
    uint4 speed;
    bool use_speed_as_volume;
    bool loop_mode;

    bool reload_decay;
    uint8 decay_counter;
    uint4 decay_volume;

    unsigned volume() const;
    void clock();
  };

  struct Sweep {
    uint8 shift;
    bool decrement;
    uint3 period;
    uint8 counter;
    bool enable;
    bool reload;
    unsigned rectangle_period;

    void clock();
  };

  struct Rectangle {
    unsigned length_counter;

    Envelope envelope;
    Sweep sweep;

    uint2 duty;
    uint3 duty_counter;

    uint11 period;
    unsigned period_counter;

    void clock_length();
    bool check_period();
    uint8 clock();
  } rectangle[2];

  struct Triangle {
    unsigned length_counter;

    uint8 linear_length;
    bool halt_length_counter;

    uint11 period;
    unsigned period_counter;

    uint8 step_counter;
    uint8 linear_length_counter;
    bool reload_linear;

    void clock_length();
    void clock_length_counter();
    uint8 clock();
  } triangle;

  struct Noise {
    unsigned length_counter;

    Envelope envelope;

    uint4 period;
    unsigned period_counter;

    bool short_mode;
    uint15 lfsr;

    void clock_length();
    uint8 clock();
  } noise;

  struct DMC {
    bool irq;
    bool loop;
    unsigned period;

    uint8 counter;
    uint16 addr;
    uint16 length;

    uint8 clock();
  } dmc;

  struct FrameCounter {
    uint2 mode;
    uint2 counter;
    signed divider;
  } frame;

  void clock_frame_counter();
  void clock_frame_counter_divider();

  uint8 enabled_channels;

  int16 rectangle_dac[32];
  int16 dmc_triangle_noise_dac[128][16][16];

  static const uint8 length_counter_table[32];
  static const uint16 ntsc_noise_period_table[16];
  static const uint16 pal_noise_period_table[16];
  static const uint16 ntsc_dmc_period_table[16];
  static const uint16 pal_dmc_period_table[16];
};

extern APU apu;
