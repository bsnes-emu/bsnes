Configuration config;

void Configuration::load() {
  configuration::load(::path.home("bsnes.cfg"));
}

void Configuration::save() {
  configuration::save(::path.home("bsnes.cfg"));
}

void Configuration::create() {
  attach(SNES::config.random, "snes.random");
  attach(SNES::config.cpu.version, "snes.cpu.version");
  attach(SNES::config.cpu.ntsc_frequency, "snes.cpu.ntscFrequency");
  attach(SNES::config.cpu.pal_frequency, "snes.cpu.palFrequency");
  attach(SNES::config.smp.ntsc_frequency, "snes.smp.ntscFrequency");
  attach(SNES::config.smp.pal_frequency, "snes.smp.palFrequency");
  attach(SNES::config.ppu1.version, "snes.ppu1.version");
  attach(SNES::config.ppu2.version, "snes.ppu2.version");
  attach(SNES::config.superfx.speed, "snes.superfx.speed");

  attach(video.driver = "", "video.driver");
  attach(video.synchronize = false, "video.synchronize");
  attach(video.smooth = true, "video.smooth");
  attach(video.filter = "", "video.filter");
  attach(video.shader = "", "video.shader");
  attach(video.region = 0, "video.region");
  attach(video.scale = 2, "video.scale");
  attach(video.fullscreenScale = 0, "video.fullscreenScale");
  attach(video.aspectRatioCorrection = true, "video.aspectRatioCorrection");
  attach(video.brightness = 100, "video.brightness");
  attach(video.contrast = 100, "video.contrast");
  attach(video.gamma = 100, "video.gamma");
  attach(video.useGammaRamp = true, "video.useGammaRamp");

  attach(audio.driver = "", "audio.driver");
  attach(audio.synchronize = true, "audio.synchronize");
  attach(audio.mute = false, "audio.mute");
  attach(audio.volume = 100, "audio.volume");
  attach(audio.balance = 100, "audio.balance");
  attach(audio.latency = 60, "audio.latency");
  attach(audio.inputFrequency = 32000, "audio.inputFrequency");
  attach(audio.outputFrequency = 44100, "audio.outputFrequency");

  attach(input.driver = "", "input.driver");

  attach(settings.startFullScreen = false, "settings.startFullScreen", "Start in full screen mode for front-end use");
  attach(settings.focusPolicy = 0, "settings.focusPolicy");
  attach(settings.compositorPolicy = 1, "settings.compositorPolicy");

  attach(controller.port1 = 1, "controller.port1");
  attach(controller.port2 = 1, "controller.port2");
}
