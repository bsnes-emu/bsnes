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

struct AudioOSS : AudioDriver {
  AudioOSS& self = *this;
  AudioOSS(Audio& super) : AudioDriver(super) {}
  ~AudioOSS() { terminate(); }

  auto create() -> bool override {
    super.setDevice("/dev/dsp");
    super.setChannels(2);
    super.setFrequency(48000);
    super.setLatency(3);
    buffer.resize(64);
    return initialize();
  }

  auto driver() -> string override { return "OSS"; }
  auto ready() -> bool override { return _fd >= 0; }

  auto hasBlocking() -> bool override { return true; }
  auto hasDynamic() -> bool override { return true; }

  auto hasDevices() -> vector<string> override {
    vector<string> devices;
    devices.append("/dev/dsp");
    for(auto& device : directory::files("/dev/", "dsp?*")) devices.append(string{"/dev/", device});
    return devices;
  }

  auto hasChannels() -> vector<uint> override {
    return {1, 2};
  }

  auto hasFrequencies() -> vector<uint> override {
    return {44100, 48000, 96000};
  }

  auto hasLatencies() -> vector<uint> override {
    return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  }

  auto setDevice(string device) -> bool override { return initialize(); }
  auto setBlocking(bool blocking) -> bool override { return updateBlocking(); }
  auto setChannels(uint channels) -> bool override { return initialize(); }
  auto setFrequency(uint frequency) -> bool override { return initialize(); }
  auto setLatency(uint latency) -> bool override { return initialize(); }

  auto clear() -> void override {
    buffer.resize(64);
  }

  auto level() -> double override {
    audio_buf_info info;
    ioctl(_fd, SNDCTL_DSP_GETOSPACE, &info);
    return (double)(_bufferSize - info.bytes) / _bufferSize;
  }

  auto output(const double samples[]) -> void override {
    for(uint n : range(self.channels)) {
      buffer.write(sclamp<16>(samples[n] * 32767.0));
      if(buffer.full()) {
        write(_fd, buffer.data(), buffer.capacity<uint8_t>());
        buffer.flush();
      }
    }
  }

private:
  auto initialize() -> bool {
    terminate();

    if(!hasDevices().find(self.device)) self.device = hasDevices().first();

    _fd = open(self.device, O_WRONLY, O_NONBLOCK);
    if(_fd < 0) return false;

    int cooked = 1;
    ioctl(_fd, SNDCTL_DSP_COOKEDMODE, &cooked);
    //policy: 0 = minimum latency (higher CPU usage); 10 = maximum latency (lower CPU usage)
    int policy = min(10, self.latency);
    ioctl(_fd, SNDCTL_DSP_POLICY, &policy);
    int channels = self.channels;
    ioctl(_fd, SNDCTL_DSP_CHANNELS, &channels);
    ioctl(_fd, SNDCTL_DSP_SETFMT, &_format);
    int frequency = self.frequency;
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
    self.blocking ? flags &=~ O_NONBLOCK : flags |= O_NONBLOCK;
    fcntl(_fd, F_SETFL, flags);
    return true;
  }

  int _fd = -1;
  int _format = AFMT_S16_LE;
  int _bufferSize = 1;

  queue<int16_t> buffer;
};
