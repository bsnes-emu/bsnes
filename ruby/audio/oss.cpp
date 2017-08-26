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

  auto availableDevices() -> string_vector {
    string_vector devices;
    devices.append("/dev/dsp");
    for(auto& device : directory::files("/dev/", "dsp?*")) devices.append(string{"/dev/", device});
    return devices;
  }

  auto availableFrequencies() -> vector<double> {
    return {44100.0, 48000.0, 96000.0};
  }

  auto availableLatencies() -> vector<uint> {
    return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  }

  auto availableChannels() -> vector<uint> {
    return {1, 2};
  }

  auto ready() -> bool { return _ready; }
  auto device() -> string { return _device; }
  auto blocking() -> bool { return _blocking; }
  auto channels() -> uint { return _channels; }
  auto frequency() -> double { return _frequency; }
  auto latency() -> uint { return _latency; }

  auto setDevice(string device) -> bool {
    if(_device == device) return true;
    _device = device;
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool {
    if(_blocking == blocking) return true;
    _blocking = blocking;
    updateBlocking();
    return true;
  }

  auto setChannels(uint channels) -> bool {
    if(_channels == channels) return true;
    _channels = channels;
    return initialize();
  }

  auto setFrequency(double frequency) -> bool {
    if(_frequency == frequency) return true;
    _frequency = frequency;
    return initialize();
  }

  auto setLatency(uint latency) -> bool {
    if(_latency == latency) return true;
    _latency = latency;
    return initialize();
  }

  auto output(const double samples[]) -> void {
    if(!ready()) return;
    for(auto n : range(_channels)) {
      int16_t sample = samples[n] * 32768.0;
      auto unused = write(_fd, &sample, 2);
    }
  }

private:
  auto initialize() -> bool {
    terminate();

    if(!availableDevices().find(_device)) {
      _device = availableDevices().left();
    }

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
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;
    if(_fd < 0) return;
    close(_fd);
    _fd = -1;
  }

  auto updateBlocking() -> void {
    if(!_ready) return;
    auto flags = fcntl(_fd, F_GETFL);
    if(flags < 0) return;
    _blocking ? flags &=~ O_NONBLOCK : flags |= O_NONBLOCK;
    fcntl(_fd, F_SETFL, flags);
  }

  bool _ready = false;
  string _device;
  bool _blocking = true;
  uint _channels = 2;
  double _frequency = 48000.0;
  uint _latency = 2;

  int _fd = -1;
  int _format = AFMT_S16_LE;
};
