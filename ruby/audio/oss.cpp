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
  ~AudioOSS() { term(); }

  struct {
    int fd = -1;
    int format = AFMT_S16_LE;
    int channels = 2;
  } device;

  struct {
    string device = "/dev/dsp";
    bool synchronize = true;
    uint frequency = 48000;
    uint latency = 60;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Audio::Device) return true;
    if(name == Audio::Synchronize) return true;
    if(name == Audio::Frequency) return true;
    if(name == Audio::Latency) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Audio::Device) return settings.device;
    if(name == Audio::Synchronize) return settings.synchronize;
    if(name == Audio::Frequency) return settings.frequency;
    if(name == Audio::Latency) return settings.latency;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Audio::Device && value.is<string>()) {
      settings.device = value.get<string>();
      if(!settings.device) settings.device = "/dev/dsp";
      return true;
    }

    if(name == Audio::Synchronize && value.is<bool>()) {
      settings.synchronize = value.get<bool>();
      updateSynchronization();
      return true;
    }

    if(name == Audio::Frequency && value.is<uint>()) {
      settings.frequency = value.get<uint>();
      if(device.fd >= 0) init();
      return true;
    }

    if(name == Audio::Latency && value.is<uint>()) {
      settings.latency = value.get<uint>();
      if(device.fd >= 0) init();
      return true;
    }

    return false;
  }

  auto sample(int16_t left, int16_t right) -> void {
    uint32_t sample = (uint16_t)left << 0 | (uint16_t)right << 16;
    auto unused = write(device.fd, &sample, 4);
  }

  auto clear() -> void {
  }

  auto init() -> bool {
    device.fd = open(settings.device, O_WRONLY, O_NONBLOCK);
    if(device.fd < 0) return false;

    int cooked = 1;
    ioctl(device.fd, SNDCTL_DSP_COOKEDMODE, &cooked);
    //policy: 0 = minimum latency (higher CPU usage); 10 = maximum latency (lower CPU usage)
    int policy = min(10, settings.latency / 20);  //note: latency measurement isn't exact
    ioctl(device.fd, SNDCTL_DSP_POLICY, &policy);
    int frequency = settings.frequency;
    ioctl(device.fd, SNDCTL_DSP_CHANNELS, &device.channels);
    ioctl(device.fd, SNDCTL_DSP_SETFMT, &device.format);
    ioctl(device.fd, SNDCTL_DSP_SPEED, &frequency);

    updateSynchronization();
    return true;
  }

  auto term() -> void {
    if(device.fd >= 0) {
      close(device.fd);
      device.fd = -1;
    }
  }

private:
  auto updateSynchronization() -> void {
    if(device.fd < 0) return;
    auto flags = fcntl(device.fd, F_GETFL);
    if(flags < 0) return;
    settings.synchronize ? flags &=~ O_NONBLOCK : flags |= O_NONBLOCK;
    fcntl(device.fd, F_SETFL, flags);
  }
};
