#if defined(AUDIO_ALSA)
  #include <ruby/audio/alsa.cpp>
#endif

#if defined(AUDIO_AO)
  #include <ruby/audio/ao.cpp>
#endif

#if defined(AUDIO_ASIO)
  #include <ruby/audio/asio.cpp>
#endif

#if defined(AUDIO_DIRECTSOUND)
  #include <ruby/audio/directsound.cpp>
#endif

#if defined(AUDIO_OPENAL)
  #include <ruby/audio/openal.cpp>
#endif

#if defined(AUDIO_OSS)
  #include <ruby/audio/oss.cpp>
#endif

#if defined(AUDIO_PULSEAUDIO)
  #include <ruby/audio/pulseaudio.cpp>
#endif

#if defined(AUDIO_PULSEAUDIOSIMPLE)
  #include <ruby/audio/pulseaudiosimple.cpp>
#endif

#if defined(AUDIO_WASAPI)
  #include <ruby/audio/wasapi.cpp>
#endif

#if defined(AUDIO_XAUDIO2)
  #include <ruby/audio/xaudio2.cpp>
#endif

namespace ruby {

auto Audio::setExclusive(bool exclusive) -> bool {
  _exclusive = exclusive;
  return true;
}

auto Audio::setContext(uintptr context) -> bool {
  _context = context;
  return true;
}

auto Audio::setDevice(string device) -> bool {
  _device = device;
  return true;
}

auto Audio::setBlocking(bool blocking) -> bool {
  _blocking = blocking;
  Audio::setFrequency(frequency());
  return true;
}

auto Audio::setDynamic(bool dynamic) -> bool {
  _dynamic = dynamic;
  return true;
}

auto Audio::setChannels(uint channels) -> bool {
  _channels = channels;
  _resamplers.reset();
  _resamplers.resize(channels);
  Audio::setFrequency(frequency());
  return true;
}

auto Audio::setFrequency(double frequency) -> bool {
  _frequency = frequency;
  for(auto& resampler : _resamplers) {
    resampler.reset(frequency, frequency);
  }
  return true;
}

auto Audio::setLatency(uint latency) -> bool {
  _latency = latency;
  return true;
}

//protected functions

auto Audio::pending() -> bool {
  return _resamplers && _resamplers[0].pending();
}

auto Audio::outputDynamic(const double samples[]) -> void {
  auto maxDelta = 0.005;
  double fillLevel = level();
  double dynamicFrequency = ((1.0 - maxDelta) + 2.0 * fillLevel * maxDelta) * frequency();
  for(auto& resampler : _resamplers) {
    resampler.setInputFrequency(dynamicFrequency);
    resampler.write(*samples++);
  }
}

//static functions

auto Audio::create(string driver) -> Audio* {
  Audio* audio = nullptr;
  if(!driver) driver = optimalDriver();

  #if defined(AUDIO_ALSA)
  if(driver == "ALSA") audio = new AudioALSA;
  #endif

  #if defined(AUDIO_AO)
  if(driver == "libao") audio = new AudioAO;
  #endif

  #if defined(AUDIO_ASIO)
  if(driver == "ASIO") audio = new AudioASIO;
  #endif

  #if defined(AUDIO_DIRECTSOUND)
  if(driver == "DirectSound") audio = new AudioDirectSound;
  #endif

  #if defined(AUDIO_OPENAL)
  if(driver == "OpenAL") audio = new AudioOpenAL;
  #endif

  #if defined(AUDIO_OSS)
  if(driver == "OSS") audio = new AudioOSS;
  #endif

  #if defined(AUDIO_PULSEAUDIO)
  if(driver == "PulseAudio") audio = new AudioPulseAudio;
  #endif

  #if defined(AUDIO_PULSEAUDIOSIMPLE)
  if(driver == "PulseAudioSimple") audio = new AudioPulseAudioSimple;
  #endif

  #if defined(AUDIO_WASAPI)
  if(driver == "WASAPI") audio = new AudioWASAPI;
  #endif

  #if defined(AUDIO_XAUDIO2)
  if(driver == "XAudio2") audio = new AudioXAudio2;
  #endif

  if(!audio) audio = new Audio;
  return audio;
}

auto Audio::optimalDriver() -> string {
  #if defined(AUDIO_ASIO)
  return "ASIO";
  #elif defined(AUDIO_WASAPI)
  return "WASAPI";
  #elif defined(AUDIO_XAUDIO2)
  return "XAudio2";
  #elif defined(AUDIO_DIRECTSOUND)
  return "DirectSound";
  #elif defined(AUDIO_ALSA)
  return "ALSA";
  #elif defined(AUDIO_OSS)
  return "OSS";
  #elif defined(AUDIO_OPENAL)
  return "OpenAL";
  #elif defined(AUDIO_PULSEAUDIO)
  return "PulseAudio";
  #elif defined(AUDIO_PULSEAUDIOSIMPLE)
  return "PulseAudioSimple";
  #elif defined(AUDIO_AO)
  return "libao";
  #else
  return "None";
  #endif
}

auto Audio::safestDriver() -> string {
  #if defined(AUDIO_DIRECTSOUND)
  return "DirectSound";
  #elif defined(AUDIO_WASAPI)
  return "WASAPI";
  #elif defined(AUDIO_XAUDIO2)
  return "XAudio2";
  #elif defined(AUDIO_ALSA)
  return "ALSA";
  #elif defined(AUDIO_OSS)
  return "OSS";
  #elif defined(AUDIO_OPENAL)
  return "OpenAL";
  #elif defined(AUDIO_PULSEAUDIO)
  return "PulseAudio";
  #elif defined(AUDIO_PULSEAUDIOSIMPLE)
  return "PulseAudioSimple";
  #elif defined(AUDIO_AO)
  return "libao";
  #elif defined(AUDIO_ASIO)
  return "ASIO";
  #else
  return "None";
  #endif
}

auto Audio::availableDrivers() -> vector<string> {
  return {

  #if defined(AUDIO_ASIO)
  "ASIO",
  #endif

  #if defined(AUDIO_WASAPI)
  "WASAPI",
  #endif

  #if defined(AUDIO_XAUDIO2)
  "XAudio2",
  #endif

  #if defined(AUDIO_DIRECTSOUND)
  "DirectSound",
  #endif

  #if defined(AUDIO_ALSA)
  "ALSA",
  #endif

  #if defined(AUDIO_OSS)
  "OSS",
  #endif

  #if defined(AUDIO_OPENAL)
  "OpenAL",
  #endif

  #if defined(AUDIO_PULSEAUDIO)
  "PulseAudio",
  #endif

  #if defined(AUDIO_PULSEAUDIOSIMPLE)
  "PulseAudioSimple",
  #endif

  #if defined(AUDIO_AO)
  "libao",
  #endif

  "None"};
}

}
