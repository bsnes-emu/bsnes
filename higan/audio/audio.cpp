#include <emulator/emulator.hpp>

namespace Emulator {

#include "stream.cpp"
Audio audio;

auto Audio::reset() -> void {
  streams.reset();
  setReverbDelay(reverbDelay);
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

auto Audio::setReverbDelay(uint reverbDelay) -> void {
  this->reverbDelay = reverbDelay;
  reverbLeft.resize(frequency * reverbDelay / 1000.0);
  reverbRight.resize(frequency * reverbDelay / 1000.0);
  memory::fill(reverbLeft.data(), reverbLeft.size() * sizeof(int16));
  memory::fill(reverbRight.data(), reverbRight.size() * sizeof(int16));
}

auto Audio::setReverbLevel(double reverbLevel) -> void {
  this->reverbLevel = reverbLevel;
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

    ileft  *= volume;
    iright *= volume;

    if(reverbDelay) {
      reverbLeft.append(ileft);
      reverbRight.append(iright);
      ileft  += reverbLeft.takeFirst()  * reverbLevel;
      iright += reverbRight.takeFirst() * reverbLevel;
    }

    interface->audioSample(sclamp<16>(ileft), sclamp<16>(iright));
  }
}

}
