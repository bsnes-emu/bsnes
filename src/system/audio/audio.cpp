#ifdef SNES_CPP

//======================
//System::AudioResampler
//======================

double System::AudioResampler::hermite(double mu1, double a, double b, double c, double d) {
  const double tension = 0.0;  //-1 = low, 0 = normal, 1 = high
  const double bias    = 0.0;  //-1 = left, 0 = even, 1 = right
  double mu2, mu3, m0, m1, a0, a1, a2, a3;

  mu2 = mu1 * mu1;
  mu3 = mu2 * mu1;

  m0  = (b - a) * (1 + bias) * (1 - tension) / 2;
  m0 += (c - b) * (1 - bias) * (1 - tension) / 2;
  m1  = (c - b) * (1 + bias) * (1 - tension) / 2;
  m1 += (d - c) * (1 - bias) * (1 - tension) / 2;

  a0 = +2 * mu3 - 3 * mu2 + 1;
  a1 =      mu3 - 2 * mu2 + mu1;
  a2 =      mu3 -     mu2;
  a3 = -2 * mu3 + 3 * mu2;

  return (a0 * b) + (a1 * m0) + (a2 * m1) + (a3 * c);
}

void System::AudioResampler::sample(uint16_t lsamp, uint16_t rsamp) {
  lhist[0] = lhist[1];
  lhist[1] = lhist[2];
  lhist[2] = lhist[3];
  lhist[3] = lsamp;

  rhist[0] = rhist[1];
  rhist[1] = rhist[2];
  rhist[2] = rhist[3];
  rhist[3] = rsamp;

  while(fraction <= 1.0) {
    unsigned p = bufferwr++;
    lbuffer[p] = sclamp<16>(hermite(fraction, lhist[0], lhist[1], lhist[2], lhist[3]));
    rbuffer[p] = sclamp<16>(hermite(fraction, rhist[0], rhist[1], rhist[2], rhist[3]));
    bufferlength++;
    fraction += step;
  }

  fraction -= 1.0;
}

void System::AudioResampler::reset(unsigned ifreq_, unsigned ofreq_) {
  ifreq = ifreq_;
  ofreq = ofreq_;

  fraction = 0;
  step = (double)ifreq / (double)ofreq;

  bufferlength = 0;
  bufferrd = 0;
  bufferwr = 0;

  for(unsigned i = 0; i < 4; i++) lhist[i] = 0, rhist[i] = 0;
}

//=============
//System::Audio
//=============

//DSP represents S-DSP core sample generation
//COP represents cartridge co-processor sample generation

//DSP frequency is typically ~32040hz
//must be valid, as SNES always generates audio
void System::Audio::set_dsp_frequency(unsigned freq) {
  dsp_frequency = freq;
  resample.reset(cop_frequency, dsp_frequency);
}

//COP frequency can be any value
//a value of zero indicates the cartridge connector sample pins are not used,
//ergo setting to zero will disable mixing between DSP and COP samples
void System::Audio::set_cop_frequency(unsigned freq) {
  cop_frequency = freq;
  resample.reset(cop_frequency, dsp_frequency);
}

//TODO: no actual mixing done yet.
//only the Super Gameboy uses this feature thus far; where it is
//possible to stop the SGB completely via disable flag.
//need a way to prevent buffer stalling when this occurs.

void System::Audio::dsp_sample(uint16 l_sample, uint16 r_sample) {
  if(cop_frequency != 0) return;

  system.interface->audio_sample(l_sample, r_sample);
}

void System::Audio::cop_sample(uint16 l_sample, uint16 r_sample) {
  if(cop_frequency == 0) return;

  resample.sample(l_sample, r_sample);
  while(resample.bufferlength) {
    unsigned p = resample.bufferrd++;
    system.interface->audio_sample(resample.lbuffer[p], resample.rbuffer[p]);
    resample.bufferlength--;
  }
}

void System::Audio::init() {
}

#endif
