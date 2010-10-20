#ifdef DSP_CPP

bool DSPDebugger::property(unsigned id, string &name, string &value) {
  unsigned n = 0;

  if(id == n++) { name = "Main Volume - Left"; value = main_volume_left(); return true; }
  if(id == n++) { name = "Main Volume - Right"; value = main_volume_right(); return true; }
  if(id == n++) { name = "Echo Volume - Left"; value = echo_volume_left(); return true; }
  if(id == n++) { name = "Echo Volume - Right"; value = echo_volume_right(); return true; }
  if(id == n++) { name = "Key On"; value = string("0x", strhex<2>(key_on())); return true; }
  if(id == n++) { name = "Key Off"; value = string("0x", strhex<2>(key_off())); return true; }
  if(id == n++) { name = "Flag - Reset"; value = flag_reset(); return true; }
  if(id == n++) { name = "Flag - Mute"; value = flag_mute(); return true; }
  if(id == n++) { name = "Flag - Echo Disable"; value = flag_echo_disable(); return true; }
  if(id == n++) { name = "Flag - Noise Clock"; value = flag_noise_clock(); return true; }
  if(id == n++) { name = "Source End Block"; value = source_end_block(); return true; }
  if(id == n++) { name = "Echo Feedback"; value = echo_feedback(); return true; }
  if(id == n++) { name = "Pitch Modulation Enable"; value = string("0x", strhex<2>(pitch_modulation_enable())); return true; }
  if(id == n++) { name = "Noise Enable"; value = string("0x", strhex<2>(noise_enable())); return true; }
  if(id == n++) { name = "Echo Enable"; value = string("0x", strhex<2>(echo_enable())); return true; }
  if(id == n++) { name = "Source Directory"; value = source_directory(); return true; }
  if(id == n++) { name = "Echo Start Address"; value = echo_start_address(); return true; }
  if(id == n++) { name = "Echo Directory"; value = echo_directory(); return true; }

  for(unsigned i = 0; i < 8; i++) {
    if(id == n++) {
      name = string("Coefficient ", i);
      value = string("0x", strhex<2>(echo_filter_coefficient(i)));
      return true;
    }
  }

  for(unsigned i = 0; i < 8; i++) {
    if(id == n++) {
      name = string("Voice ", i);
      value = "";
      return true;
    }

    if(id == n++) { name = "Volume - Left"; value = voice_volume_left(i); return true; }
    if(id == n++) { name = "Volume - Right"; value = voice_volume_right(i); return true; }
    if(id == n++) { name = "Pitch Height"; value = string("0x", strhex<4>(voice_pitch_height(i))); return true; }
    if(id == n++) { name = "Source Number"; value = voice_source_number(i); return true; }
    if(id == n++) { name = "ADSR1"; value = voice_adsr1(i); return true; }
    if(id == n++) { name = "ADSR2"; value = voice_adsr2(i); return true; }
    if(id == n++) { name = "GAIN"; value = voice_gain(i); return true; }
    if(id == n++) { name = "ENVX"; value = voice_envx(i); return true; }
    if(id == n++) { name = "OUTX"; value = voice_outx(i); return true; }
  }

  return false;
}

//===========
//DSPDebugger
//===========

unsigned DSPDebugger::main_volume_left() { return state.regs[0x0c]; }
unsigned DSPDebugger::main_volume_right() { return state.regs[0x1c]; }
unsigned DSPDebugger::echo_volume_left() { return state.regs[0x2c]; }
unsigned DSPDebugger::echo_volume_right() { return state.regs[0x3c]; }
unsigned DSPDebugger::key_on() { return state.regs[0x4c]; }
unsigned DSPDebugger::key_off() { return state.regs[0x5c]; }
bool DSPDebugger::flag_reset() { return state.regs[0x6c] & 0x80; }
bool DSPDebugger::flag_mute() { return state.regs[0x6c] & 0x40; }
bool DSPDebugger::flag_echo_disable() { return state.regs[0x6c] & 0x20; }
unsigned DSPDebugger::flag_noise_clock() { return state.regs[0x6c] & 0x1f; }
unsigned DSPDebugger::source_end_block() { return state.regs[0x7c]; }
unsigned DSPDebugger::echo_feedback() { return state.regs[0x0d]; }
unsigned DSPDebugger::pitch_modulation_enable() { return state.regs[0x2d]; }
unsigned DSPDebugger::noise_enable() { return state.regs[0x3d]; }
unsigned DSPDebugger::echo_enable() { return state.regs[0x4d]; }
unsigned DSPDebugger::source_directory() { return state.regs[0x5d]; }
unsigned DSPDebugger::echo_start_address() { return state.regs[0x6d]; }
unsigned DSPDebugger::echo_directory() { return state.regs[0x7d]; }
unsigned DSPDebugger::echo_filter_coefficient(unsigned n) { return state.regs[(n << 4) + 0x0f]; }
unsigned DSPDebugger::voice_volume_left(unsigned n) { return state.regs[(n << 4) + 0x00]; }
unsigned DSPDebugger::voice_volume_right(unsigned n) { return state.regs[(n << 4) + 0x01]; }
unsigned DSPDebugger::voice_pitch_height(unsigned n) { return state.regs[(n << 4) + 0x02] + (state.regs[(n << 4) + 0x03] << 8); }
unsigned DSPDebugger::voice_source_number(unsigned n) { return state.regs[(n << 4) + 0x04]; }
unsigned DSPDebugger::voice_adsr1(unsigned n) { return state.regs[(n << 4) + 0x05]; }
unsigned DSPDebugger::voice_adsr2(unsigned n) { return state.regs[(n << 4) + 0x06]; }
unsigned DSPDebugger::voice_gain(unsigned n) { return state.regs[(n << 4) + 0x07]; }
unsigned DSPDebugger::voice_envx(unsigned n) { return state.regs[(n << 4) + 0x08]; }
unsigned DSPDebugger::voice_outx(unsigned n) { return state.regs[(n << 4) + 0x09]; }

#endif
