#include <AL/al.h>
#include <AL/alc.h>

namespace ruby {

#include "openal.hpp"

class pAudioOpenAL {
public:
  AudioOpenAL &self;

  struct {
    ALCdevice *handle;
    ALCcontext *context;
    ALuint source;
    ALenum format;
    unsigned latency;
    unsigned queue_length;
  } device;

  struct {
    uint32_t *data;
    unsigned length;
    unsigned size;
  } buffer;

  struct {
    bool synchronize;
    unsigned frequency;
    unsigned latency;
  } settings;

  bool cap(Audio::Setting setting) {
    if(setting == Audio::Synchronize) return true;
    if(setting == Audio::Frequency) return true;
    if(setting == Audio::Latency) return true;
    return false;
  }

  uintptr_t get(Audio::Setting setting) {
    if(setting == Audio::Synchronize) return settings.synchronize;
    if(setting == Audio::Frequency) return settings.frequency;
    if(setting == Audio::Latency) return settings.latency;
    return false;
  }

  bool set(Audio::Setting setting, uintptr_t param) {
    if(setting == Audio::Synchronize) {
      settings.synchronize = param;
      return true;
    }

    if(setting == Audio::Frequency) {
      settings.frequency = param;
      return true;
    }

    if(setting == Audio::Latency) {
      if(settings.latency != param) {
        settings.latency = param;
        update_latency();
      }
      return true;
    }

    return false;
  }

  void sample(uint16_t sl, uint16_t sr) {
    buffer.data[buffer.length++] = sl + (sr << 16);
    if(buffer.length < buffer.size) return;

    ALuint albuffer = 0;
    int processed = 0;
    while(true) {
      alGetSourcei(device.source, AL_BUFFERS_PROCESSED, &processed);
      while(processed--) {
        alSourceUnqueueBuffers(device.source, 1, &albuffer);
        alDeleteBuffers(1, &albuffer);
        device.queue_length--;
      }
      //wait for buffer playback to catch up to sample generation if not synchronizing
      if(settings.synchronize == false || device.queue_length < 3) break;
    }

    if(device.queue_length < 3) {
      alGenBuffers(1, &albuffer);
      alBufferData(albuffer, device.format, buffer.data, buffer.size * 4, settings.frequency);
      alSourceQueueBuffers(device.source, 1, &albuffer);
      device.queue_length++;
    }

    ALint playing;
    alGetSourcei(device.source, AL_SOURCE_STATE, &playing);
    if(playing != AL_PLAYING) alSourcePlay(device.source);
    buffer.length = 0;
  }

  void update_latency() {
    if(buffer.data) delete[] buffer.data;
    buffer.size = settings.frequency * settings.latency / 1000.0 + 0.5;
    buffer.data = new uint32_t[buffer.size];
  }

  bool init() {
    update_latency();
    device.queue_length = 0;

    bool success = false;
    if(device.handle = alcOpenDevice(NULL)) {
      if(device.context = alcCreateContext(device.handle, NULL)) {
        alcMakeContextCurrent(device.context);
        alGenSources(1, &device.source);

        //alSourcef (device.source, AL_PITCH, 1.0);
        //alSourcef (device.source, AL_GAIN, 1.0);
        //alSource3f(device.source, AL_POSITION, 0.0, 0.0, 0.0);
        //alSource3f(device.source, AL_VELOCITY, 0.0, 0.0, 0.0);
        //alSource3f(device.source, AL_DIRECTION, 0.0, 0.0, 0.0);
        //alSourcef (device.source, AL_ROLLOFF_FACTOR, 0.0);
        //alSourcei (device.source, AL_SOURCE_RELATIVE, AL_TRUE);

        alListener3f(AL_POSITION, 0.0, 0.0, 0.0);
        alListener3f(AL_VELOCITY, 0.0, 0.0, 0.0);
        ALfloat listener_orientation[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        alListenerfv(AL_ORIENTATION, listener_orientation);

        success = true;
      }
    }

    if(success == false) {
      term();
      return false;
    }

    return true;
  }

  void term() {
    if(alIsSource(device.source) == AL_TRUE) {
      int playing = 0;
      alGetSourcei(device.source, AL_SOURCE_STATE, &playing);
      if(playing == AL_PLAYING) {
        alSourceStop(device.source);
        int queued = 0;
        alGetSourcei(device.source, AL_BUFFERS_QUEUED, &queued);
        while(queued--) {
          ALuint albuffer = 0;
          alSourceUnqueueBuffers(device.source, 1, &albuffer);
          alDeleteBuffers(1, &albuffer);
          device.queue_length--;
        }
      }

      alDeleteSources(1, &device.source);
      device.source = 0;
    }

    if(device.context) {
      alcMakeContextCurrent(NULL);
      alcDestroyContext(device.context);
      device.context = 0;
    }

    if(device.handle) {
      alcCloseDevice(device.handle);
      device.handle = 0;
    }

    if(buffer.data) {
      delete[] buffer.data;
      buffer.data = 0;
    }
  }

  pAudioOpenAL(AudioOpenAL &self_) : self(self_) {
    device.source = 0;
    device.handle = 0;
    device.context = 0;
    device.format = AL_FORMAT_STEREO16;
    device.queue_length = 0;

    buffer.data = 0;
    buffer.length = 0;
    buffer.size = 0;

    settings.synchronize = true;
    settings.frequency = 22050;
    settings.latency = 40;
  }

  ~pAudioOpenAL() {
    term();
  }
};

bool AudioOpenAL::cap(Setting setting) { return p.cap(setting); }
uintptr_t AudioOpenAL::get(Setting setting) { return p.get(setting); }
bool AudioOpenAL::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
void AudioOpenAL::sample(uint16_t sl, uint16_t sr) { p.sample(sl, sr); }
bool AudioOpenAL::init() { return p.init(); }
void AudioOpenAL::term() { p.term(); }
AudioOpenAL::AudioOpenAL() : p(*new pAudioOpenAL(*this)) {}
AudioOpenAL::~AudioOpenAL() { delete &p; }

} //namespace ruby
