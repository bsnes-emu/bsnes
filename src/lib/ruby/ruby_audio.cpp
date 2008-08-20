bool AudioInterface::init() {
  if(!p) driver();
  return p->init();
}

void AudioInterface::term() {
  if(p) {
    delete p;
    p = 0;
  }
}

bool AudioInterface::cap(Audio::Setting setting) {
  if(setting == Audio::Volume) return true;
  if(setting == Audio::Resample) return true;
  if(setting == Audio::ResampleOutputFrequency) return true;
  if(setting == Audio::ResampleInputFrequency) return true;

  return p ? p->cap(setting) : false;
}

uintptr_t AudioInterface::get(Audio::Setting setting) {
  if(setting == Audio::Volume) return volume;
  if(setting == Audio::Resample) return resample_enabled;
  if(setting == Audio::ResampleOutputFrequency) return r_outfreq;
  if(setting == Audio::ResampleInputFrequency) return r_infreq;

  return p ? p->get(setting) : false;
}

bool AudioInterface::set(Audio::Setting setting, uintptr_t param) {
  if(setting == Audio::Volume) {
    volume = param;
    return true;
  }

  if(setting == Audio::Resample) {
    resample_enabled = param;
    return true;
  }

  if(setting == Audio::ResampleOutputFrequency) {
    r_outfreq = max(1, param);
    r_step = (double)r_infreq / (double)r_outfreq;
    r_frac = 0;
    return true;
  }

  if(setting == Audio::ResampleInputFrequency) {
    r_infreq = max(1, param);
    r_step = (double)r_infreq / (double)r_outfreq;
    r_frac = 0;
    return true;
  }

  return p ? p->set(setting, param) : false;
}

//4-tap hermite interpolation
double AudioInterface::hermite(double mu1, double a, double b, double c, double d) {
  const double tension = 0.0; //-1 = low, 0 = normal, 1 = high
  const double bias    = 0.0; //-1 = left, 0 = even, 1 = right

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

void AudioInterface::sample(uint16_t left, uint16_t right) {
  if(volume != 100) {
    left  = sclamp<16>((double)left  * (double)volume / 100.0);
    right = sclamp<16>((double)right * (double)volume / 100.0);
  }

  r_left [0] = r_left [1];
  r_left [1] = r_left [2];
  r_left [2] = r_left [3];
  r_left [3] = (int16_t)left;

  r_right[0] = r_right[1];
  r_right[1] = r_right[2];
  r_right[2] = r_right[3];
  r_right[3] = (int16_t)right;

  if(resample_enabled == false) {
    if(p) p->sample(left, right);
    return;
  }

  while(r_frac <= 1.0) {
    int output_left  = sclamp<16>(hermite(r_frac, r_left [0], r_left [1], r_left [2], r_left [3]));
    int output_right = sclamp<16>(hermite(r_frac, r_right[0], r_right[1], r_right[2], r_right[3]));
    r_frac += r_step;
    if(p) p->sample(output_left, output_right);
  }

  r_frac -= 1.0;
}

void AudioInterface::clear() {
  r_frac = 0;
  r_left [0] = r_left [1] = r_left [2] = r_left [3] = 0;
  r_right[0] = r_right[1] = r_right[2] = r_right[3] = 0;
  if(p) p->clear();
}

AudioInterface::AudioInterface() {
  p = 0;
  volume = 100;
  resample_enabled = false;
  r_outfreq = r_infreq = 1;
  r_step = r_frac = 0;
  r_left [0] = r_left [1] = r_left [2] = r_left [3] = 0;
  r_right[0] = r_right[1] = r_right[2] = r_right[3] = 0;
}

AudioInterface::~AudioInterface() {
  term();
}
