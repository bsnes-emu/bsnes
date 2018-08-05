#if defined(PLATFORM_MACOS)
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#else
  #include <AL/al.h>
  #include <AL/alc.h>
#endif

struct AudioOpenAL : AudioDriver {
  AudioOpenAL& self = *this;
  AudioOpenAL(Audio& driver) : AudioDriver(super) {}
  ~AudioOpenAL() { terminate(); }

  auto create() -> bool override {
    super.setDevice(hasDevices().first());
    super.setChannels(2);
    super.setFrequency(48000);
    super.setLatency(20);
    return initialize();
  }

  auto driver() -> string override { return "OpenAL"; }
  auto ready() -> bool override { return _ready; }

  auto hasBlocking() -> bool override { return true; }

  auto hasDevices() -> vector<string> override {
    vector<string> devices;
    if(const char* list = alcGetString(nullptr, ALC_DEVICE_SPECIFIER)) {
      while(list && *list) {
        devices.append(list);
        list += strlen(list) + 1;
      }
    }
    return devices;
  }

  auto hasChannels() -> vector<uint> override {
    return {2};
  }

  auto hasFrequencies() -> vector<uint> override {
    return {44100, 48000, 96000};
  }

  auto hasLatencies() -> vector<uint> override {
    return {20, 40, 60, 80, 100};
  }

  auto setDevice(string device) -> bool override { return initialize(); }
  auto setBlocking(bool blocking) -> bool override { return true; }
  auto setFrequency(uint frequency) -> bool override { return initialize(); }
  auto setLatency(uint latency) -> bool override { return updateLatency(); }

  auto output(const double samples[]) -> void override {
    _buffer[_bufferLength]  = (uint16_t)sclamp<16>(samples[0] * 32767.0) <<  0;
    _buffer[_bufferLength] |= (uint16_t)sclamp<16>(samples[1] * 32767.0) << 16;
    if(++_bufferLength < _bufferSize) return;

    ALuint alBuffer = 0;
    int processed = 0;
    while(true) {
      alGetSourcei(_source, AL_BUFFERS_PROCESSED, &processed);
      while(processed--) {
        alSourceUnqueueBuffers(_source, 1, &alBuffer);
        alDeleteBuffers(1, &alBuffer);
        _queueLength--;
      }
      //wait for buffer playback to catch up to sample generation if not synchronizing
      if(!self.blocking || _queueLength < 3) break;
    }

    if(_queueLength < 3) {
      alGenBuffers(1, &alBuffer);
      alBufferData(alBuffer, _format, _buffer, _bufferSize * 4, self.frequency);
      alSourceQueueBuffers(_source, 1, &alBuffer);
      _queueLength++;
    }

    ALint playing;
    alGetSourcei(_source, AL_SOURCE_STATE, &playing);
    if(playing != AL_PLAYING) alSourcePlay(_source);
    _bufferLength = 0;
  }

private:
  auto initialize() -> bool {
    terminate();

    if(!hasDevices().find(self.device)) self.device = hasDevices().first();
    _queueLength = 0;
    updateLatency();

    bool success = false;
    if(_openAL = alcOpenDevice(self.device)) {
      if(_context = alcCreateContext(_openAL, nullptr)) {
        alcMakeContextCurrent(_context);
        alGenSources(1, &_source);

      //alSourcef (_source, AL_PITCH, 1.0);
      //alSourcef (_source, AL_GAIN, 1.0);
      //alSource3f(_source, AL_POSITION, 0.0, 0.0, 0.0);
      //alSource3f(_source, AL_VELOCITY, 0.0, 0.0, 0.0);
      //alSource3f(_source, AL_DIRECTION, 0.0, 0.0, 0.0);
      //alSourcef (_source, AL_ROLLOFF_FACTOR, 0.0);
      //alSourcei (_source, AL_SOURCE_RELATIVE, AL_TRUE);

        alListener3f(AL_POSITION, 0.0, 0.0, 0.0);
        alListener3f(AL_VELOCITY, 0.0, 0.0, 0.0);
        ALfloat listenerOrientation[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        alListenerfv(AL_ORIENTATION, listenerOrientation);

        success = true;
      }
    }

    if(!success) return terminate(), false;
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;

    if(alIsSource(_source) == AL_TRUE) {
      int playing = 0;
      alGetSourcei(_source, AL_SOURCE_STATE, &playing);
      if(playing == AL_PLAYING) {
        alSourceStop(_source);
        int queued = 0;
        alGetSourcei(_source, AL_BUFFERS_QUEUED, &queued);
        while(queued--) {
          ALuint alBuffer = 0;
          alSourceUnqueueBuffers(_source, 1, &alBuffer);
          alDeleteBuffers(1, &alBuffer);
          _queueLength--;
        }
      }

      alDeleteSources(1, &_source);
      _source = 0;
    }

    if(_context) {
      alcMakeContextCurrent(nullptr);
      alcDestroyContext(_context);
      _context = nullptr;
    }

    if(_openAL) {
      alcCloseDevice(_openAL);
      _openAL = nullptr;
    }

    delete[] _buffer;
    _buffer = nullptr;
  }

  auto updateLatency() -> bool {
    delete[] _buffer;
    _bufferSize = self.frequency * self.latency / 1000.0 + 0.5;
    _buffer = new uint32_t[_bufferSize]();
    return true;
  }

  bool _ready = false;

  ALCdevice* _openAL = nullptr;
  ALCcontext* _context = nullptr;
  ALuint _source = 0;
  ALenum _format = AL_FORMAT_STEREO16;
  uint _queueLength = 0;

  uint32_t* _buffer = nullptr;
  uint _bufferLength = 0;
  uint _bufferSize = 0;
};
