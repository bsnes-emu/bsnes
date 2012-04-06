void APU::runsequencer() {
  auto &r = sequencer;

  if(r.base == 0) {  //512hz
    if(r.step == 0 || r.step == 2 || r.step == 4 || r.step == 6) {  //256hz
      square1.clocklength();
      square2.clocklength();
      wave.clocklength();
      noise.clocklength();
    }
    if(r.step == 2 || r.step == 6) {  //128hz
      square1.clocksweep();
    }
    if(r.step == 7) {  //64hz
      square1.clockenvelope();
      square2.clockenvelope();
      noise.clockenvelope();
    }
    r.step++;
  }
  r.base++;

  if(r.enable[0]) square1.run();
  if(r.enable[1]) square2.run();
  if(r.enable[2]) wave.run();
  if(r.enable[3]) noise.run();

  signed lsample = 0;
  if(r.lenable[0]) lsample += square1.output;
  if(r.lenable[1]) lsample += square2.output;
  if(r.lenable[2]) lsample +=    wave.output;
  if(r.lenable[3]) lsample +=   noise.output;
  lsample = (lsample * 512) - 15360;
  lsample = r.volumeadjust(lsample, r.lvolume);
  r.lsample = lsample;

  signed rsample = 0;
  if(r.renable[0]) rsample += square1.output;
  if(r.renable[1]) rsample += square2.output;
  if(r.renable[2]) rsample +=    wave.output;
  if(r.renable[3]) rsample +=   noise.output;
  rsample = (rsample * 512) - 15360;
  rsample = r.volumeadjust(rsample, r.rvolume);
  r.rsample = rsample;

  if(r.masterenable == false) {
    r.lsample = 0;
    r.rsample = 0;
  }
}

signed APU::Sequencer::volumeadjust(signed sample, uint3 volume) {
  switch(volume) {
  case 0: return (sample >> 3);                  // 12.5%
  case 1: return (sample >> 2);                  // 25.0%
  case 2: return (sample >> 2) + (sample >> 3);  // 37.5%
  case 3: return (sample >> 1);                  // 50.0%
  case 4: return (sample >> 1) + (sample >> 3);  // 62.5%
  case 5: return (sample >> 0) - (sample >> 2);  // 75.0%
  case 6: return (sample >> 0) - (sample >> 3);  // 87.5%
  case 7: return (sample >> 0);                  //100.0%
  }
}

uint8 APU::Sequencer::read(unsigned addr) const {
  switch(addr) {
  case 0: return (rvolume << 0) | (lvolume << 4);
  case 1: return (
    (renable[0] << 0)
  | (renable[1] << 1)
  | (renable[2] << 2)
  | (renable[3] << 3)
  | (lenable[0] << 4)
  | (lenable[1] << 5)
  | (lenable[2] << 6)
  | (lenable[3] << 7)
  );
  case 2: return (
    (enable[0]    << 0)
  | (enable[1]    << 1)
  | (enable[2]    << 2)
  | (enable[3]    << 3)
  | (masterenable << 7)
  );
  }
}

void APU::Sequencer::write(unsigned addr, uint8 byte) {
  switch(addr) {
  case 0:  //NR50
    rvolume = byte >> 0;
    lvolume = byte >> 4;
    break;

  case 1:  //NR51
    renable[0] = byte >> 0;
    renable[1] = byte >> 1;
    renable[2] = byte >> 2;
    renable[3] = byte >> 3;
    lenable[0] = byte >> 4;
    lenable[1] = byte >> 5;
    lenable[2] = byte >> 6;
    lenable[3] = byte >> 7;
    break;

  case 2:  //NR52
    enable[0]    = byte >> 0;
    enable[1]    = byte >> 1;
    enable[2]    = byte >> 2;
    enable[3]    = byte >> 3;
    masterenable = byte >> 7;
    break;
  }
}

void APU::Sequencer::power() {
  lvolume = 0;
  rvolume = 0;
  for(auto &n : lenable) n = 0;
  for(auto &n : renable) n = 0;
  for(auto &n : enable) n = 0;
  masterenable = 0;
  base = 0;
  step = 0;
  lsample = 0;
  rsample = 0;
}
