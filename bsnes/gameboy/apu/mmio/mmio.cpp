#ifdef APU_CPP

uint8 APU::mmio_read(uint16 addr) {
  if(addr == 0xff10) {  //NR10
    return (channel1.sweep_time << 4)
         | (channel1.sweep_direction << 3)
         | (channel1.sweep_shift << 0);
  }

  if(addr == 0xff11) {  //NR11
    return (channel1.wave_pattern_duty << 6);
  }

  if(addr == 0xff12) {  //NR12
    return (channel1.initial_envelope_volume << 4)
         | (channel1.envelope_direction << 3)
         | (channel1.envelope_sweep << 0);
  }

  if(addr == 0xff14) {  //NR14
    return (channel1.consecutive_selection << 6);
  }

  if(addr == 0xff16) {  //NR21
    return (channel2.wave_pattern_duty << 6);
  }

  if(addr == 0xff17) {  //NR22
    return (channel2.initial_envelope_volume << 4)
         | (channel2.envelope_direction << 3)
         | (channel2.envelope_sweep << 0);
  }

  if(addr == 0xff19) {  //NR24
    return (channel2.consecutive_selection << 6);
  }

  if(addr == 0xff1a) {  //NR30
    return (channel3.off << 7);
  }

  if(addr == 0xff1b) {  //NR31
    return (channel3.sound_length << 0);
  }

  if(addr == 0xff1c) {  //NR32
    return (channel3.output_level << 5);
  }

  if(addr == 0xff1e) {  //NR34
    return (channel3.consecutive_selection << 6);
  }

  if(addr == 0xff20) {  //NR41
    return (channel4.sound_length << 0);
  }

  if(addr == 0xff21) {  //NR42
    return (channel4.initial_envelope_volume << 4)
         | (channel4.envelope_direction << 3)
         | (channel4.envelope_sweep << 0);
  }

  if(addr == 0xff22) {  //NR43
    return (channel4.shift_clock_frequency << 4)
         | (channel4.counter_step_width << 3)
         | (channel4.dividing_ratio << 0);
  }

  if(addr == 0xff23) {  //NR44
    return (channel4.consecutive_selection << 6);
  }

  if(addr == 0xff24) {  //NR50
    return (control.output_vin_to_so2 << 7)
         | (control.so2_output_level << 4)
         | (control.output_vin_to_so1 << 3)
         | (control.so1_output_level << 0);
  }

  if(addr == 0xff25) {  //NR51
    return (control.output_channel4_to_so2 << 7)
         | (control.output_channel3_to_so2 << 6)
         | (control.output_channel2_to_so2 << 5)
         | (control.output_channel1_to_so2 << 4)
         | (control.output_channel4_to_so1 << 3)
         | (control.output_channel3_to_so1 << 2)
         | (control.output_channel2_to_so1 << 1)
         | (control.output_channel1_to_so1 << 0);
  }

  if(addr == 0xff26) {  //NR52
    return (control.sound_on << 7);
  }

  if(addr >= 0xff30 && addr <= 0xff3f) {
    return channel3.pattern[addr & 15];
  }

  return 0x00;
}

void APU::mmio_write(uint16 addr, uint8 data) {
  if(addr == 0xff10) {  //NR10
    channel1.sweep_time = (data >> 4) & 7;
    channel1.sweep_direction = data & 0x08;
    channel1.sweep_shift = data & 0x07;
    return;
  }

  if(addr == 0xff11) {  //NR11
    channel1.wave_pattern_duty = (data >> 6) & 3;
    channel1.sound_length = data & 0x3f;
    return;
  }

  if(addr == 0xff12) {  //NR12
    channel1.initial_envelope_volume = (data >> 4) & 15;
    channel1.envelope_direction = data & 0x08;
    channel1.envelope_sweep = data & 0x07;
    return;
  }

  if(addr == 0xff13) {  //NR13
    channel1.frequency = (channel1.frequency & 0x0700) | (data << 0);
    return;
  }

  if(addr == 0xff14) {  //NR14
    channel1.initialize = data & 0x80;
    channel1.consecutive_selection = data & 0x40;
    channel1.frequency = ((data & 7) << 8) | (channel1.frequency & 0x00ff);
    return;
  }

  if(addr == 0xff16) {  //NR21
    channel2.wave_pattern_duty = (data >> 6) & 3;
    channel2.sound_length = data & 0x3f;
    return;
  }

  if(addr == 0xff17) {  //NR22
    channel2.initial_envelope_volume = (data >> 4) & 15;
    channel2.envelope_direction = data & 0x08;
    channel2.envelope_sweep = data & 0x07;
    return;
  }

  if(addr == 0xff18) {  //NR23
    channel2.frequency = (channel2.frequency & 0x0700) | (data << 0);
    return;
  }

  if(addr == 0xff19) {  //NR24
    channel2.initialize = data & 0x80;
    channel2.consecutive_selection = data & 0x40;
    channel2.frequency = ((data & 7) << 8) | (channel2.frequency & 0x00ff);
    return;
  }

  if(addr == 0xff1a) {  //NR30
    channel3.off = data & 0x80;
    return;
  }

  if(addr == 0xff1b) {  //NR31
    channel3.sound_length = data;
    return;
  }

  if(addr == 0xff1c) {  //NR32
    channel3.output_level = (data >> 5) & 3;
    return;
  }

  if(addr == 0xff1d) {  //NR33
    channel3.frequency = (channel3.frequency & 0x0700) | (data << 0);
    return;
  }

  if(addr == 0xff1e) {  //NR34
    channel3.initialize = data & 0x80;
    channel3.consecutive_selection = data & 0x40;
    channel3.frequency = ((data & 7) << 8) | (channel3.frequency & 0x00ff);
    return;
  }

  if(addr == 0xff20) {  //NR41
    channel4.sound_length = data & 0x3f;
    return;
  }

  if(addr == 0xff21) {  //NR42
    channel4.initial_envelope_volume = (data >> 3) & 15;
    channel4.envelope_direction = data & 0x08;
    channel4.envelope_sweep = data & 0x07;
    return;
  }

  if(addr == 0xff22) {  //NR43
    channel4.shift_clock_frequency = (data >> 4) & 15;
    channel4.counter_step_width = data & 0x08;
    channel4.dividing_ratio = data & 0x07;
    return;
  }

  if(addr == 0xff23) {  //NR44
    channel4.initialize = data & 0x80;
    channel4.consecutive_selection = data & 0x40;
    return;
  }

  if(addr == 0xff24) {  //NR50
    control.output_vin_to_so2 = data & 0x80;
    control.so2_output_level = (data >> 4) & 7;
    control.output_vin_to_so1 = data & 0x08;
    control.so1_output_level = (data >> 0) & 7;
    return;
  }

  if(addr == 0xff25) {  //NR51
    control.output_channel4_to_so2 = data & 0x80;
    control.output_channel3_to_so2 = data & 0x40;
    control.output_channel2_to_so2 = data & 0x20;
    control.output_channel1_to_so2 = data & 0x10;
    control.output_channel4_to_so1 = data & 0x08;
    control.output_channel3_to_so1 = data & 0x04;
    control.output_channel2_to_so1 = data & 0x02;
    control.output_channel1_to_so1 = data & 0x01;
    return;
  }

  if(addr == 0xff26) {  //NR52
    control.sound_on = data & 0x80;
    control.channel4_on = data & 0x08;
    control.channel3_on = data & 0x04;
    control.channel2_on = data & 0x02;
    control.channel1_on = data & 0x01;
    return;
  }

  if(addr >= 0xff30 && addr <= 0xff3f) {
    channel3.pattern[addr & 15] = data;
    return;
  }
}

#endif
