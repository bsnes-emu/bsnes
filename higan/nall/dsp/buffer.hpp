#ifdef NALL_DSP_INTERNAL_HPP

struct Buffer {
  double** sample = nullptr;
  uint16_t rdoffset = 0;
  uint16_t wroffset = 0;
  unsigned channels = 0;

  void setChannels(unsigned channels) {
    if(sample) {
      for(unsigned c = 0; c < this->channels; c++) {
        if(sample[c]) delete[] sample[c];
      }
      delete[] sample;
    }

    this->channels = channels;
    if(channels == 0) return;

    sample = new double*[channels];
    for(unsigned c = 0; c < channels; c++) {
      sample[c] = new double[65536]();
    }
  }

  inline double& read(unsigned channel, signed offset = 0) {
    return sample[channel][(uint16_t)(rdoffset + offset)];
  }

  inline double& write(unsigned channel, signed offset = 0) {
    return sample[channel][(uint16_t)(wroffset + offset)];
  }

  inline void clear() {
    for(unsigned c = 0; c < channels; c++) {
      for(unsigned n = 0; n < 65536; n++) {
        sample[c][n] = 0;
      }
    }
    rdoffset = 0;
    wroffset = 0;
  }

  Buffer() {
  }

  ~Buffer() {
    setChannels(0);
  }
};

#endif
