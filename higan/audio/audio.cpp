#include <emulator/emulator.hpp>

namespace Emulator {

#include "stream.cpp"
Audio audio;

auto Audio::reset(maybe<uint> channels_, maybe<double> frequency_) -> void {
  if(channels_) channels = channels_();
  if(frequency_) frequency = frequency_();

  streams.reset();
  reverb.reset();

  reverb.resize(channels);
  for(auto c : range(channels)) {
    reverb[c].resize(7);
    reverb[c][0].resize(1229);
    reverb[c][1].resize(1559);
    reverb[c][2].resize(1907);
    reverb[c][3].resize(4057);
    reverb[c][4].resize(8117);
    reverb[c][5].resize(8311);
    reverb[c][6].resize(9931);
  }
}

auto Audio::setInterface(Interface* interface) -> void {
  this->interface = interface;
}

auto Audio::setVolume(double volume) -> void {
  this->volume = volume;
}

auto Audio::setBalance(double balance) -> void {
  this->balance = balance;
}

auto Audio::setReverb(bool enabled) -> void {
  this->reverbEnable = enabled;
}

auto Audio::createStream(uint channels, double frequency) -> shared_pointer<Stream> {
  shared_pointer<Stream> stream = new Stream;
  stream->reset(channels, frequency, this->frequency);
  streams.append(stream);
  return stream;
}

auto Audio::process() -> void {
  while(true) {
    for(auto& stream : streams) {
      if(!stream->pending()) return;
    }

    double samples[channels] = {0};
    for(auto& stream : streams) {
      double buffer[16];
      uint length = stream->read(buffer), offset = 0;

      for(auto c : range(channels)) {
        samples[c] += buffer[offset];
        if(++offset >= length) offset = 0;
      }
    }

    for(auto c : range(channels)) {
      samples[c] /= streams.size();

      if(reverbEnable) {
        samples[c] *= 0.125;
        for(auto n : range(7)) samples[c] += 0.125 * reverb[c][n].last();
        for(auto n : range(7)) reverb[c][n].write(samples[c]);
        samples[c] *= 8.000;
      }

      samples[c] *= volume;
    }

    if(channels == 2) {
      if(balance < 0.0) samples[1] *= 1.0 + balance;
      if(balance > 0.0) samples[0] *= 1.0 - balance;
    }

    interface->audioSample(samples, channels);
  }
}

}
