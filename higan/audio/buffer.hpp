#pragma once

struct Buffer {
  Buffer() {
  }

  ~Buffer() {
    setChannels(0);
  }

  auto setChannels(uint channels) -> void {
    if(sample) {
      for(auto c : range(this->channels)) {
        if(sample[c]) delete[] sample[c];
      }
      delete[] sample;
    }

    this->channels = channels;
    if(channels == 0) return;

    sample = new double*[channels];
    for(auto c : range(channels)) {
      sample[c] = new double[65536]();
    }
  }

  inline auto read(uint channel, int offset = 0) -> double& {
    return sample[channel][(uint16_t)(rdoffset + offset)];
  }

  inline auto write(uint channel, int offset = 0) -> double& {
    return sample[channel][(uint16_t)(wroffset + offset)];
  }

  inline auto clear() -> void {
    for(auto c : range(channels)) {
      for(auto n : range(65536)) {
        sample[c][n] = 0;
      }
    }
    rdoffset = 0;
    wroffset = 0;
  }

  double** sample = nullptr;
  uint16_t rdoffset = 0;
  uint16_t wroffset = 0;
  uint channels = 0;
};
