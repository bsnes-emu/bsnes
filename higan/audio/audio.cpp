#include <emulator/emulator.hpp>

namespace Emulator {

#include "stream.cpp"
Audio audio;

auto Audio::reset() -> void {
  streams.reset();
  reverb.reset();
  reverb.resize(2);
  for(auto c : range(2)) {
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

auto Audio::setFrequency(double frequency) -> void {
  this->frequency = frequency;
  for(auto& stream : streams) stream->setFrequency(frequency);
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
  shared_pointer<Stream> stream = new Stream{channels, frequency};
  stream->setFrequency(this->frequency);
  streams.append(stream);
  return stream;
}

//audio mixer
auto Audio::poll() -> void {
  while(true) {
    for(auto& stream : streams) {
      if(!stream->pending()) return;
    }

    double left = 0.0, right = 0.0;
    for(auto& stream : streams) {
      double samples[2];
      stream->read(samples);
      left  += samples[0];
      right += samples[1];
    }
    left  /= streams.size();
    right /= streams.size();

    if(balance < 0.0) right *= 1.0 + balance;
    if(balance > 0.0) left  *= 1.0 - balance;

    //todo: apply volume, reverb before denormalization?
    int ileft  = (left  * 65535.0) - 32768.0;
    int iright = (right * 65535.0) - 32768.0;

    if(reverbEnable) {
      ileft *= 0.125;
      for(auto n : range(7)) ileft += 0.125 * reverb[0][n].last();
      for(auto n : range(7)) reverb[0][n].write(ileft);
      ileft *= 8.000;

      iright *= 0.125;
      for(auto n : range(7)) iright += 0.125 * reverb[1][n].last();
      for(auto n : range(7)) reverb[1][n].write(iright);
      iright *= 8.000;
    }

    ileft  *= volume;
    iright *= volume;

    interface->audioSample(sclamp<16>(ileft), sclamp<16>(iright));
  }
}

}
