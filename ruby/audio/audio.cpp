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
  if(driver->exclusive == exclusive) return true;
  if(!driver->hasExclusive()) return false;
  if(!driver->setExclusive(driver->exclusive = exclusive)) return false;
  return true;
}

auto Audio::setContext(uintptr context) -> bool {
  if(driver->context == context) return true;
  if(!driver->hasContext()) return false;
  if(!driver->setContext(driver->context = context)) return false;
  return true;
}

auto Audio::setDevice(string device) -> bool {
  if(driver->device == device) return true;
  if(!driver->hasDevice(device)) return false;
  if(!driver->setDevice(driver->device = device)) return false;
  return true;
}

auto Audio::setBlocking(bool blocking) -> bool {
  if(driver->blocking == blocking) return true;
  if(!driver->hasBlocking()) return false;
  if(!driver->setBlocking(driver->blocking = blocking)) return false;
  for(auto& resampler : resamplers) resampler.reset(driver->frequency);
  return true;
}

auto Audio::setDynamic(bool dynamic) -> bool {
  if(driver->dynamic == dynamic) return true;
  if(!driver->hasDynamic()) return false;
  if(!driver->setDynamic(driver->dynamic = dynamic)) return false;
  return true;
}

auto Audio::setChannels(uint channels) -> bool {
  if(driver->channels == channels) return true;
  if(!driver->hasChannels(channels)) return false;
  if(!driver->setChannels(driver->channels = channels)) return false;
  resamplers.reset();
  resamplers.resize(channels);
  for(auto& resampler : resamplers) resampler.reset(driver->frequency);
  return true;
}

auto Audio::setFrequency(uint frequency) -> bool {
  if(driver->frequency == frequency) return true;
  if(!driver->hasFrequency(frequency)) return false;
  if(!driver->setFrequency(driver->frequency = frequency)) return false;
  for(auto& resampler : resamplers) resampler.reset(driver->frequency);
  return true;
}

auto Audio::setLatency(uint latency) -> bool {
  if(driver->latency == latency) return true;
  if(!driver->hasLatency(latency)) return false;
  if(!driver->setLatency(driver->latency = latency)) return false;
  return true;
}

//

auto Audio::clear() -> void {
  for(auto& resampler : resamplers) resampler.reset(driver->frequency);
  return driver->clear();
}

auto Audio::level() -> double {
  return driver->level();
}

auto Audio::output(const double samples[]) -> void {
  if(!driver->dynamic) return driver->output(samples);

  auto maxDelta = 0.005;
  double fillLevel = driver->level();
  double dynamicFrequency = ((1.0 - maxDelta) + 2.0 * fillLevel * maxDelta) * driver->frequency;
  for(auto& resampler : resamplers) {
    resampler.setInputFrequency(dynamicFrequency);
    resampler.write(*samples++);
  }

  while(resamplers.first().pending()) {
    double samples[driver->channels];
    for(uint n : range(driver->channels)) samples[n] = resamplers[n].read();
    driver->output(samples);
  }
}

//

auto Audio::create(string driver) -> bool {
  reset();
  if(!driver) driver = optimalDriver();

  #if defined(AUDIO_ALSA)
  if(driver == "ALSA") self.driver = new AudioALSA(*this);
  #endif

  #if defined(AUDIO_AO)
  if(driver == "libao") self.driver = new AudioAO(*this);
  #endif

  #if defined(AUDIO_ASIO)
  if(driver == "ASIO") self.driver = new AudioASIO(*this);
  #endif

  #if defined(AUDIO_DIRECTSOUND)
  if(driver == "DirectSound") self.driver = new AudioDirectSound(*this);
  #endif

  #if defined(AUDIO_OPENAL)
  if(driver == "OpenAL") self.driver = new AudioOpenAL(*this);
  #endif

  #if defined(AUDIO_OSS)
  if(driver == "OSS") self.driver = new AudioOSS(*this);
  #endif

  #if defined(AUDIO_PULSEAUDIO)
  if(driver == "PulseAudio") self.driver = new AudioPulseAudio(*this);
  #endif

  #if defined(AUDIO_PULSEAUDIOSIMPLE)
  if(driver == "PulseAudioSimple") self.driver = new AudioPulseAudioSimple(*this);
  #endif

  #if defined(AUDIO_WASAPI)
  if(driver == "WASAPI") self.driver = new AudioWASAPI(*this);
  #endif

  #if defined(AUDIO_XAUDIO2)
  if(driver == "XAudio2") self.driver = new AudioXAudio2(*this);
  #endif

  if(!self.driver) self.driver = new AudioDriver(*this);

  return self.driver->create();
}

auto Audio::hasDrivers() -> vector<string> {
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

}
