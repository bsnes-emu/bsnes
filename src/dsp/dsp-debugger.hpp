struct DSPDebugger : ChipDebugger {
  bool property(unsigned id, string &name, string &value);

  virtual unsigned main_volume_left() { return 0; }
  virtual unsigned main_volume_right() { return 0; }
  virtual unsigned echo_volume_left() { return 0; }
  virtual unsigned echo_volume_right() { return 0; }
  virtual unsigned key_on() { return 0; }
  virtual unsigned key_off() { return 0; }
  virtual bool flag_reset() { return 0; }
  virtual bool flag_mute() { return 0; }
  virtual bool flag_echo_disable() { return 0; }
  virtual unsigned flag_noise_clock() { return 0; }
  virtual unsigned source_end_block() { return 0; }
  virtual unsigned echo_feedback() { return 0; }
  virtual unsigned pitch_modulation_enable() { return 0; }
  virtual unsigned noise_enable() { return 0; }
  virtual unsigned echo_enable() { return 0; }
  virtual unsigned source_directory() { return 0; }
  virtual unsigned echo_start_address() { return 0; }
  virtual unsigned echo_directory() { return 0; }
  virtual unsigned echo_filter_coefficient(unsigned) { return 0; }
  virtual unsigned voice_volume_left(unsigned) { return 0; }
  virtual unsigned voice_volume_right(unsigned) { return 0; }
  virtual unsigned voice_pitch_height(unsigned) { return 0; }
  virtual unsigned voice_source_number(unsigned) { return 0; }
  virtual unsigned voice_adsr1(unsigned) { return 0; }
  virtual unsigned voice_adsr2(unsigned) { return 0; }
  virtual unsigned voice_gain(unsigned) { return 0; }
  virtual unsigned voice_envx(unsigned) { return 0; }
  virtual unsigned voice_outx(unsigned) { return 0; }
};
