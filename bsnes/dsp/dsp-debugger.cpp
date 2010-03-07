#ifdef DSP_CPP

bool DSPDebugger::property(unsigned id, string &name, string &value) {
  unsigned n = 0;

  if(id == n++) { name = "Main Volume - Left"; value = main_volume_left(); return true; }
  if(id == n++) { name = "Main Volume - Right"; value = main_volume_right(); return true; }
  if(id == n++) { name = "Echo Volume - Left"; value = echo_volume_left(); return true; }
  if(id == n++) { name = "Echo Volume - Right"; value = echo_volume_right(); return true; }
  if(id == n++) { name = "Key On"; value = sprint("0x$", strhex<2>(key_on())); return true; }
  if(id == n++) { name = "Key Off"; value = sprint("0x$", strhex<2>(key_off())); return true; }
  if(id == n++) { name = "Flag - Reset"; value = flag_reset(); return true; }
  if(id == n++) { name = "Flag - Mute"; value = flag_mute(); return true; }
  if(id == n++) { name = "Flag - Echo Disable"; value = flag_echo_disable(); return true; }
  if(id == n++) { name = "Flag - Noise Clock"; value = flag_noise_clock(); return true; }
  if(id == n++) { name = "Source End Block"; value = source_end_block(); return true; }
  if(id == n++) { name = "Echo Feedback"; value = echo_feedback(); return true; }
  if(id == n++) { name = "Pitch Modulation Enable"; value = sprint("0x$", strhex<2>(pitch_modulation_enable())); return true; }
  if(id == n++) { name = "Noise Enable"; value = sprint("0x$", strhex<2>(noise_enable())); return true; }
  if(id == n++) { name = "Echo Enable"; value = sprint("0x$", strhex<2>(echo_enable())); return true; }
  if(id == n++) { name = "Source Directory"; value = source_directory(); return true; }
  if(id == n++) { name = "Echo Start Address"; value = echo_start_address(); return true; }
  if(id == n++) { name = "Echo Directory"; value = echo_directory(); return true; }

  for(unsigned i = 0; i < 8; i++) {
    if(id == n++) {
      name = string() << "Coefficient " << i;
      value = sprint("0x$", strhex<2>(echo_filter_coefficient(i)));
      return true;
    }
  }

  for(unsigned i = 0; i < 8; i++) {
    if(id == n++) {
      name = string() << "Voice " << i;
      value = "";
      return true;
    }

    if(id == n++) { name = "Volume - Left"; value = voice_volume_left(i); return true; }
    if(id == n++) { name = "Volume - Right"; value = voice_volume_right(i); return true; }
    if(id == n++) { name = "Pitch Height"; value = sprint("0x$", strhex<4>(voice_pitch_height(i))); return true; }
    if(id == n++) { name = "Source Number"; value = voice_source_number(i); return true; }
    if(id == n++) { name = "ADSR1"; value = voice_adsr1(i); return true; }
    if(id == n++) { name = "ADSR2"; value = voice_adsr2(i); return true; }
    if(id == n++) { name = "GAIN"; value = voice_gain(i); return true; }
    if(id == n++) { name = "ENVX"; value = voice_envx(i); return true; }
    if(id == n++) { name = "OUTX"; value = voice_outx(i); return true; }
  }

  return false;
}

#endif
