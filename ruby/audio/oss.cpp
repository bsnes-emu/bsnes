#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

//OSSv4 features: define fallbacks for OSSv3 (where these ioctls are ignored)

#ifndef SNDCTL_DSP_COOKEDMODE
  #define SNDCTL_DSP_COOKEDMODE _IOW('P', 30, int)
#endif

#ifndef SNDCTL_DSP_POLICY
  #define SNDCTL_DSP_POLICY _IOW('P', 45, int)
#endif

struct AudioOSS : Audio {
  AudioOSS() { initialize(); }
  ~AudioOSS() { terminate(); }

  auto driver() -> string override {
    return "OSS";
  }

  auto ready() -> bool override {
    return _fd >= 0;
  }

  auto availableDevices() -> vector<string> override {
    vector<string> devices;
    devices.append("/dev/dsp");
    for(auto& device : directory::files("/dev/", "dsp?*")) devices.append(string{"/dev/", device});
    return devices;
  }

  auto availableChannels() -> vector<uint> override {
    return {1, 2};
  }

  auto availableFrequencies() -> vector<double> override {
    return {44100.0, 48000.0, 96000.0};
  }

  auto availableLatencies() -> vector<uint> override {
    return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  }

  auto hasDevice() -> bool override { return true; }
  auto hasDynamic() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }
  auto hasChannels() -> bool override { return true; }
  auto hasFrequency() -> bool override { return true; }
  auto hasLatency() -> bool override { return true; }

  auto setDevice(string device) -> bool override {
    if(device == this->device()) return true;
    if(!Audio::setDevice(device)) return false;
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool override {
    if(blocking == this->blocking()) return true;
    if(!Audio::setBlocking(blocking)) return false;
    return updateBlocking();
  }

  auto setDynamic(bool dynamic) -> bool override {
    if(dynamic == this->dynamic()) return true;
    if(!Audio::setDynamic(dynamic)) return false;
    return true;
  }

  auto setChannels(uint channels) -> bool override {
    if(channels == this->channels()) return true;
    if(!Audio::setChannels(channels)) return false;
    return initialize();
  }

  auto setFrequency(double frequency) -> bool override {
    if(frequency == this->frequency()) return true;
    if(!Audio::setFrequency(frequency)) return false;
    return initialize();
  }

  auto setLatency(uint latency) -> bool override {
    if(latency == this->latency()) return true;
    if(!Audio::setLatency(latency)) return false;
    return initialize();
  }

  auto level() -> double override {
    audio_buf_info info;
    ioctl(_fd, SNDCTL_DSP_GETOSPACE, &info);
    return (double)(_bufferSize - info.bytes) / _bufferSize;
  }

  auto output(const double samples[]) -> void override {
    if(!ready()) return;

    if(!_dynamic) {
      for(uint n : range(channels())) {
        sample(sclamp<16>(samples[n] * 32767.0));
      }
    } else {
      Audio::outputDynamic(samples);
      while(pending()) {
        for(auto& resampler : _resamplers) {
          auto sample = (uint16_t)sclamp<16>(resampler.read() * 32767.0);
          auto unused = write(_fd, &sample, 2);
        }
      }
    }
  }

private:
  auto initialize() -> bool {
    terminate();

    if(!availableDevices().find(_device)) {
      Audio::setDevice(availableDevices().left());
    }
    Audio::setChannels(channels());

    _fd = open(_device, O_WRONLY, O_NONBLOCK);
    if(_fd < 0) return false;

    int cooked = 1;
    ioctl(_fd, SNDCTL_DSP_COOKEDMODE, &cooked);
    //policy: 0 = minimum latency (higher CPU usage); 10 = maximum latency (lower CPU usage)
    int policy = min(10, _latency);
    ioctl(_fd, SNDCTL_DSP_POLICY, &policy);
    int channels = _channels;
    ioctl(_fd, SNDCTL_DSP_CHANNELS, &channels);
    ioctl(_fd, SNDCTL_DSP_SETFMT, &_format);
    int frequency = _frequency;
    ioctl(_fd, SNDCTL_DSP_SPEED, &frequency);
    updateBlocking();
    audio_buf_info info;
    ioctl(_fd, SNDCTL_DSP_GETOSPACE, &info);
    _bufferSize = info.bytes;

    return true;
  }

  auto terminate() -> void {
    if(!ready()) return;
    close(_fd);
    _fd = -1;
  }

  auto updateBlocking() -> bool {
    if(!ready()) return false;
    auto flags = fcntl(_fd, F_GETFL);
    if(flags < 0) return false;
    _blocking ? flags &=~ O_NONBLOCK : flags |= O_NONBLOCK;
    fcntl(_fd, F_SETFL, flags);
    return true;
  }

  auto sample(uint16_t sample) -> void {
    _outputBuffer[_outputOffset++] = sample;
    if(_outputOffset >= sizeof(_outputBuffer) / sizeof(uint16_t)) {
      write(_fd, &_outputBuffer, sizeof(_outputBuffer));
      _outputOffset = 0;
    }
  }

  int _fd = -1;
  int _format = AFMT_S16_LE;
  int _bufferSize = 1;

  uint _outputOffset = 0;
  uint16_t _outputBuffer[256];
};
