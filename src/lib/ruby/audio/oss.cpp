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

#include <ruby/ruby.h>

namespace ruby {

#include "oss.h"

class pAudioOSS {
public:
  AudioOSS &self;

  struct {
    int fd;
    int format;
    int channels;
    const char *name;
  } device;

  struct {
    unsigned frequency;
  } settings;

  bool cap(Audio::Setting setting) {
    if(setting == Audio::Frequency) return true;
    return false;
  }

  uintptr_t get(Audio::Setting setting) {
    if(setting == Audio::Frequency) return settings.frequency;
    return false;
  }

  bool set(Audio::Setting setting, uintptr_t param) {
    if(setting == Audio::Frequency) {
      settings.frequency = param;
      if(device.fd > 0) {
        term();
        init();
      }
      return true;
    }
    return false;
  }

  void sample(uint16_t sl, uint16_t sr) {
    uint32_t sample = sl + (sr << 16);
    write(device.fd, &sample, 4);
  }

  bool init() {
    device.fd = open(device.name, O_WRONLY, O_NONBLOCK);
    if(device.fd < 0) return false;

    #if 1 //SOUND_VERSION >= 0x040000
    //attempt to enable OSS4-specific features regardless of version
    //OSS3 ioctl calls will silently fail, but sound will still work
    int cooked = 1, policy = 4; //policy should be 0 - 10, lower = less latency, more CPU usage
    ioctl(device.fd, SNDCTL_DSP_COOKEDMODE, &cooked);
    ioctl(device.fd, SNDCTL_DSP_POLICY, &policy);
    #endif
    int freq = settings.frequency;
    ioctl(device.fd, SNDCTL_DSP_CHANNELS, &device.channels);
    ioctl(device.fd, SNDCTL_DSP_SETFMT, &device.format);
    ioctl(device.fd, SNDCTL_DSP_SPEED, &freq);

    return true;
  }

  void term() {
    if(device.fd > 0) {
      close(device.fd);
      device.fd = -1;
    }
  }

  pAudioOSS(AudioOSS &self_) : self(self_) {
    device.fd = -1;
    device.format = AFMT_S16_LE;
    device.channels = 2;
    device.name = "/dev/dsp";

    settings.frequency = 22050;
  }

  ~pAudioOSS() {
    term();
  }
};

bool AudioOSS::cap(Setting setting) { return p.cap(setting); }
uintptr_t AudioOSS::get(Setting setting) { return p.get(setting); }
bool AudioOSS::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
void AudioOSS::sample(uint16_t sl, uint16_t sr) { p.sample(sl, sr); }
bool AudioOSS::init() { return p.init(); }
void AudioOSS::term() { p.term(); }
AudioOSS::AudioOSS() : p(*new pAudioOSS(*this)) {}
AudioOSS::~AudioOSS() { delete &p; }

} //namespace ruby
