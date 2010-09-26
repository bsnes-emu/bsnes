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

  attach(video.driver = "", "video.driver");
  attach(video.synchronize = false, "video.synchronize");
  attach(video.contrast = 100, "video.contrast");
  attach(video.brightness = 100, "video.brightness");
  attach(video.gamma = 100, "video.gamma");
  attach(video.useGammaRamp = true, "video.useGammaRamp");

  attach(audio.driver = "", "audio.driver");
  attach(audio.synchronize = true, "audio.synchronize");
  attach(audio.mute = false, "audio.mute");

  attach(input.driver = "", "input.driver");

  attach(settings.focusPolicy = 0, "settings.focusPolicy");
  attach(settings.useNativeDialogs = false, "settings.useNativeDialogs");
}
