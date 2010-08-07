class sDSPDebugger : public sDSP, public DSPDebugger {
public:
  //===========
  //DSPDebugger
  //===========

  unsigned main_volume_left();
  unsigned main_volume_right();
  unsigned echo_volume_left();
  unsigned echo_volume_right();
  unsigned key_on();
  unsigned key_off();
  bool flag_reset();
  bool flag_mute();
  bool flag_echo_disable();
  unsigned flag_noise_clock();
  unsigned source_end_block();
  unsigned echo_feedback();
  unsigned pitch_modulation_enable();
  unsigned noise_enable();
  unsigned echo_enable();
  unsigned source_directory();
  unsigned echo_start_address();
  unsigned echo_directory();
  unsigned echo_filter_coefficient(unsigned);
  unsigned voice_volume_left(unsigned);
  unsigned voice_volume_right(unsigned);
  unsigned voice_pitch_height(unsigned);
  unsigned voice_source_number(unsigned);
  unsigned voice_adsr1(unsigned);
  unsigned voice_adsr2(unsigned);
  unsigned voice_gain(unsigned);
  unsigned voice_envx(unsigned);
  unsigned voice_outx(unsigned);
};
