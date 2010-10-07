Configuration config;

void Configuration::load() {
  configuration::load(string(path.user, "bsnes-phoenix.cfg"));
}

void Configuration::save() {
  mkdir(path.user, 0755);
  configuration::save(string(path.user, "bsnes-phoenix.cfg"));
}

void Configuration::create() {
  attach(path.current = "", "path.current");
  attach(path.satellaviewBios = "", "path.satellaviewBios");
  attach(path.sufamiTurboBios = "", "path.sufamiTurboBios");
  attach(path.superGameBoyBios = "", "path.superGameBoyBios");

  attach(video.driver = "", "video.driver");
  attach(video.synchronize = false, "video.synchronize");
  attach(video.smooth = true, "video.smooth");
  attach(video.shader = "", "video.shader");
  attach(video.region = 0, "video.region");
  attach(video.scale = 2, "video.scale");
  attach(video.aspectRatioCorrection = true, "video.aspectRatioCorrection");
  attach(video.brightness = 100, "video.brightness");
  attach(video.contrast = 100, "video.contrast");
  attach(video.gamma = 100, "video.gamma");
  attach(video.useGammaRamp = true, "video.useGammaRamp");

  attach(audio.driver = "", "audio.driver");
  attach(audio.synchronize = true, "audio.synchronize");
  attach(audio.mute = false, "audio.mute");
  attach(audio.volume = 100, "audio.volume");
  attach(audio.latency = 60, "audio.latency");
  attach(audio.inputFrequency = 32000, "audio.inputFrequency");
  attach(audio.outputFrequency = 44100, "audio.outputFrequency");

  attach(input.driver = "", "input.driver");

  attach(settings.focusPolicy = 0, "settings.focusPolicy");

  attach(controller.port1 = 1, "controller.port1");
  attach(controller.port2 = 1, "controller.port2");
}
