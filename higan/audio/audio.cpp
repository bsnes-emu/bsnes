#include <emulator/emulator.hpp>

namespace Emulator {

Audio audio;

Stream::Stream(double inputFrequency, double outputFrequency, double volume, double balance) {
  dsp.setChannels(2);
  dsp.setPrecision(16);
  dsp.setFrequency(inputFrequency);
  dsp.setResampler(DSP::ResampleEngine::Sinc);
  dsp.setResamplerFrequency(outputFrequency);
  dsp.setVolume(volume);
  dsp.setBalance(balance);
}

auto Stream::sample(int16 left, int16 right) -> void {
  int samples[] = {left, right};
  dsp.sample(samples);
  audio.poll();
}

//

auto Audio::reset() -> void {
  streams.reset();
  setReverbDelay(reverbDelay);
}

auto Audio::setInterface(Interface* interface) -> void {
  this->interface = interface;
}

auto Audio::setFrequency(double frequency) -> void {
  this->frequency = frequency;
  for(auto& stream : streams) stream->dsp.setResamplerFrequency(frequency);
}

auto Audio::setVolume(double volume) -> void {
  this->volume = volume;
  for(auto& stream : streams) stream->dsp.setVolume(volume);
}

auto Audio::setBalance(double balance) -> void {
  this->balance = balance;
  for(auto& stream : streams) stream->dsp.setBalance(balance);
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

auto Audio::createStream(double frequency) -> shared_pointer<Stream> {
  shared_pointer<Stream> stream = new Stream{frequency, this->frequency, volume, balance};
  streams.append(stream);
  return stream;
}

//audio mixer
auto Audio::poll() -> void {
  while(true) {
    for(auto& stream : streams) {
      if(!stream->dsp.pending()) return;
    }

    int left = 0, right = 0;
    for(auto& stream : streams) {
      int samples[2];
      stream->dsp.read(samples);
      left += samples[0];
      right += samples[1];
    }

    if(reverbDelay) {
      reverbLeft.append(left);
      reverbRight.append(right);
      left += reverbLeft.takeFirst() * reverbLevel;
      right += reverbRight.takeFirst() * reverbLevel;
    }

    interface->audioSample(sclamp<16>(left), sclamp<16>(right));
  }
}

}
