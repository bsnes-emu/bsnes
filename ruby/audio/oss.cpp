#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

//OSS4 soundcard.h includes below SNDCTL defines, but OSS3 does not
//However, OSS4 soundcard.h does not reside in <sys/>
//Therefore, attempt to manually define SNDCTL values if using OSS3 header
//Note that if the defines below fail to work on any specific platform, one can point soundcard.h
//above to the correct location for OSS4 (usually /usr/lib/oss/include/sys/soundcard.h)
//Failing that, one can disable OSS4 ioctl calls inside init() and remove the below defines

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
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Audio::Device) return true;
    if(name == Audio::Synchronize) return true;
    if(name == Audio::Frequency) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Audio::Device) return settings.device;
    if(name == Audio::Synchronize) return settings.synchronize;
    if(name == Audio::Frequency) return settings.frequency;
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

    return false;
  }

  auto sample(uint16_t left, uint16_t right) -> void {
    uint32_t sample = left << 0 | right << 16;
    auto unused = write(device.fd, &sample, 4);
  }

  auto clear() -> void {
  }

  auto init() -> bool {
    device.fd = open(settings.device, O_WRONLY, O_NONBLOCK);
    if(device.fd < 0) return false;

    #if 1 //SOUND_VERSION >= 0x040000
    //attempt to enable OSS4-specific features regardless of version
    //OSS3 ioctl calls will silently fail, but sound will still work
    int cooked = 1, policy = 4; //policy should be 0 - 10, lower = less latency, more CPU usage
    ioctl(device.fd, SNDCTL_DSP_COOKEDMODE, &cooked);
    ioctl(device.fd, SNDCTL_DSP_POLICY, &policy);
    #endif
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
