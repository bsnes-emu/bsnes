uint8 mmio_read(uint16 addr);
void mmio_write(uint16 addr, uint8 data);

struct Channel1 {  //tone and sweep
  //$ff10  NR10
  unsigned sweep_time;
  bool sweep_direction;
  unsigned sweep_shift;

  //$ff11  NR11
  unsigned wave_pattern_duty;
  unsigned sound_length;

  //$ff12  NR12
  unsigned initial_envelope_volume;
  bool envelope_direction;
  unsigned envelope_sweep;

  //$ff13,$ff14  NR13,NR14
  unsigned frequency;
  bool initialize;
  bool consecutive_selection;
} channel1;

struct Channel2 {  //tone
  //$ff16  NR21
  unsigned wave_pattern_duty;
  unsigned sound_length;

  //$ff17  NR22
  unsigned initial_envelope_volume;
  bool envelope_direction;
  unsigned envelope_sweep;

  //$ff18,$ff19  NR23,NR24
  unsigned frequency;
  bool initialize;
  bool consecutive_selection;
} channel2;

struct Channel3 {  //wave output
  //$ff1a  NR30
  bool off;

  //$ff1b  NR31
  unsigned sound_length;

  //$ff1c  NR32
  unsigned output_level;

  //$ff1d,$ff1e  NR33,NR34
  unsigned frequency;
  bool initialize;
  bool consecutive_selection;

  //$ff30-ff3f
  uint8 pattern[16];
} channel3;

struct Channel4 {  //noise
  //$ff20  NR41
  unsigned sound_length;

  //$ff21  NR42
  unsigned initial_envelope_volume;
  bool envelope_direction;
  unsigned envelope_sweep;

  //$ff22  NR43
  unsigned shift_clock_frequency;
  bool counter_step_width;
  unsigned dividing_ratio;

  //$ff23  NR44
  bool initialize;
  bool consecutive_selection;
} channel4;

struct Control {
  //$ff24  NR50
  bool output_vin_to_so2;
  unsigned so2_output_level;
  bool output_vin_to_so1;
  unsigned so1_output_level;

  //$ff25  NR51
  bool output_channel4_to_so2;
  bool output_channel3_to_so2;
  bool output_channel2_to_so2;
  bool output_channel1_to_so2;
  bool output_channel4_to_so1;
  bool output_channel3_to_so1;
  bool output_channel2_to_so1;
  bool output_channel1_to_so1;

  //$ff26  NR52
  bool sound_on;
  bool channel4_on;
  bool channel3_on;
  bool channel2_on;
  bool channel1_on;
} control;
