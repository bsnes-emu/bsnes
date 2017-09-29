#if defined(PLATFORM_MACOS)
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#else
  #include <AL/al.h>
  #include <AL/alc.h>
#endif

struct AudioOpenAL : Audio {
  AudioOpenAL() { initialize(); }
  ~AudioOpenAL() { terminate(); }

  auto availableDevices() -> string_vector {
    string_vector devices;
    for(auto& device : queryDevices()) devices.append(device);
    return devices;
  }

  auto availableFrequencies() -> vector<double> {
    return {44100.0, 48000.0, 96000.0};
  }

  auto availableLatencies() -> vector<uint> {
    return {20, 40, 60, 80, 100};
  }

  auto availableChannels() -> vector<uint> {
    return {2};
  }

  auto ready() -> bool { return _ready; }
  auto device() -> string { return _device; }
  auto blocking() -> bool { return _blocking; }
  auto channels() -> uint { return _channels; }
  auto frequency() -> double { return (double)_frequency; }
  auto latency() -> uint { return _latency; }

  auto setDevice(string device) -> bool {
    if(_device == device) return true;
    _device = device;
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool {
    if(_blocking == blocking) return true;
    _blocking = blocking;
    return true;
  }

  auto setFrequency(double frequency) -> bool {
    if(_frequency == (uint)frequency) return true;
    _frequency = (uint)frequency;
    return initialize();
  }

  auto setLatency(uint latency) -> bool {
    if(_latency == latency) return true;
    _latency = latency;
    if(_ready) updateLatency();
    return true;
  }

  auto output(const double samples[]) -> void {
    _buffer[_bufferLength]  = int16_t(samples[0] * 32768.0) <<  0;
    _buffer[_bufferLength] |= int16_t(samples[1] * 32768.0) << 16;
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

  auto queryDevices() -> string_vector {
    string_vector result;

    const char* list = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
    if(!list) return result;

    while(list && *list) {
      result.append(list);
      list += strlen(list) + 1;
    }

    return result;
  }

  auto updateLatency() -> void {
    delete[] _buffer;
    _bufferSize = _frequency * _latency / 1000.0 + 0.5;
    _buffer = new uint32_t[_bufferSize]();
  }

  bool _ready = false;
  string _device;
  bool _blocking = true;
  uint _channels = 2;
  uint _frequency = 48000;
  uint _latency = 20;

  ALCdevice* _openAL = nullptr;
  ALCcontext* _context = nullptr;
  ALuint _source = 0;
  ALenum _format = AL_FORMAT_STEREO16;
  uint _queueLength = 0;

  uint32_t* _buffer = nullptr;
  uint _bufferLength = 0;
  uint _bufferSize = 0;
};
