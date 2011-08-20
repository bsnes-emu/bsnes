#ifdef NALL_DSP_INTERNAL_HPP

struct Buffer {
  double *sample[2];
  uint16_t rdoffset;
  uint16_t wroffset;

  inline double& read(bool channel, signed offset = 0) {
    return sample[channel][(uint16_t)(rdoffset + offset)];
  }

  inline double& write(bool channel, signed offset = 0) {
    return sample[channel][(uint16_t)(wroffset + offset)];
  }

  inline void clear() {
    for(unsigned n = 0; n < 65536; n++) {
      sample[0][n] = 0;
      sample[1][n] = 0;
    }
    rdoffset = 0;
    wroffset = 0;
  }

  Buffer() {
    sample[0] = new double[65536];
    sample[1] = new double[65536];
  }

  ~Buffer() {
    delete[] sample[0];
    delete[] sample[1];
  }
};

#endif
