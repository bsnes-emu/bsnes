#if defined(PLATFORM_MACOS)
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#else
  #include <AL/al.h>
  #include <AL/alc.h>
#endif

struct AudioOpenAL : Audio {
  AudioOpenAL() {
    Audio::setDevice(availableDevices().first());
    Audio::setChannels(2);
    Audio::setFrequency(48000.0);
    Audio::setLatency(20);
    initialize();
  }

  ~AudioOpenAL() {
    terminate();
  }

  auto driver() -> string override { return "OpenAL"; }
  auto ready() -> bool override { return _ready; }

  auto hasDevice() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }
  auto hasChannels() -> bool override { return true; }
  auto hasFrequency() -> bool override { return true; }
  auto hasLatency() -> bool override { return true; }

  auto availableDevices() -> vector<string> override {
    vector<string> devices;
    if(const char* list = alcGetString(nullptr, ALC_DEVICE_SPECIFIER)) {
      while(list && *list) {
        result.append(list);
        list += strlen(list) + 1;
      }
    }
    return devices;
  }

  auto availableChannels() -> vector<uint> override {
    return {2};
  }

  auto availableFrequencies() -> vector<double> override {
    return {44100.0, 48000.0, 96000.0};
  }

  auto availableLatencies() -> vector<uint> override {
    return {20, 40, 60, 80, 100};
  }

  auto context() -> uintptr override { return 0; }
  auto dynamic() -> bool override { return false; }

  auto setDevice(string device) -> bool override {
    if(device == Audio::device()) return true;
    if(!Audio::setDevice(device)) return false;
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool override {
    if(blocking == Audio::blocking()) return true;
    if(!Audio::setBlocking(blocking)) return false;
    return true;
  }

  auto setFrequency(double frequency) -> bool override {
    if(frequency == Audio::frequency()) return true;
    if(!Audio::setFrequency(frequency)) return false;
    return initialize();
  }

  auto setLatency(uint latency) -> bool override {
    if(latency == Audio::latency()) return true;
    if(!Audio::setLatency(latency)) return false;
    if(ready()) updateLatency();
    return true;
  }

  auto write(const double samples[]) -> void override {
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
      if(!_blocking || _queueLength < 3) break;
    }

    if(_queueLength < 3) {
      alGenBuffers(1, &alBuffer);
      alBufferData(alBuffer, _format, _buffer, _bufferSize * 4, _frequency);
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

    if(!queryDevices().find(_device)) _device = "";
    _queueLength = 0;
    updateLatency();

    bool success = false;
    if(_openAL = alcOpenDevice(_device)) {
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

  auto updateLatency() -> void {
    delete[] _buffer;
    _bufferSize = _frequency * _latency / 1000.0 + 0.5;
    _buffer = new uint32_t[_bufferSize]();
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
