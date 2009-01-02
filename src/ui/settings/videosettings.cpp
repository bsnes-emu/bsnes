void VideoSettingsWindow::setup() {
  create(0, 451, 370);

  lcontrast.create  (0, 451, 18);
  contrast.create   (0, 451, 30, 191);
  lbrightness.create(0, 451, 18);
  brightness.create (0, 451, 30, 191);
  lgamma.create     (0, 451, 18);
  gamma.create      (0, 451, 30, 191);
  gamma_ramp.create (0, 223, 18, translate["Gamma ramp"]);
  sepia.create      (0, 223, 18, translate["Sepia"]);
  grayscale.create  (0, 223, 18, translate["Grayscale"]);
  invert.create     (0, 223, 18, translate["Invert colors"]);
  sync_ui();

  unsigned y = 0;
  attach(lcontrast,     0, y); y += 18;
  attach(contrast,      0, y); y += 30;
  attach(lbrightness,   0, y); y += 18;
  attach(brightness,    0, y); y += 30;
  attach(lgamma,        0, y); y += 18;
  attach(gamma,         0, y); y += 30;

  attach(gamma_ramp,    0, y);
  attach(sepia,       228, y); y += 18;
  attach(grayscale,     0, y);
  attach(invert,      228, y); y += 18 + 5;

  contrast.on_change   = bind(&VideoSettingsWindow::contrast_change, this);
  brightness.on_change = bind(&VideoSettingsWindow::brightness_change, this);
  gamma.on_change      = bind(&VideoSettingsWindow::gamma_change, this);
  gamma_ramp.on_tick   = bind(&VideoSettingsWindow::gammaramp_tick, this);
  sepia.on_tick        = bind(&VideoSettingsWindow::sepia_tick, this);
  grayscale.on_tick    = bind(&VideoSettingsWindow::grayscale_tick, this);
  invert.on_tick       = bind(&VideoSettingsWindow::invert_tick, this);
}

//update all UI controls to match config file values ...
void VideoSettingsWindow::sync_ui() {
  contrast.set_position(config::system.contrast + 95);
  lcontrast.set_text(string()
  << translate["Contrast adjust"] << ": "
  << ((int)config::system.contrast > 0 ? "+" : "")
  << (int)config::system.contrast << "%");
  brightness.set_position(config::system.brightness + 95);
  lbrightness.set_text(string()
  << translate["Brightness adjust"] << ": "
  << ((int)config::system.brightness > 0 ? "+" : "")
  << (int)config::system.brightness << "%");
  gamma.set_position(config::system.gamma - 5);
  lgamma.set_text(string()
  << translate["Gamma adjust"] << ": "
  << ((int)config::system.gamma > 100 ? "+" : "")
  << (int)(config::system.gamma - 100) << "%");
  gamma_ramp.check(config::system.gamma_ramp);
  sepia.check(config::system.sepia);
  grayscale.check(config::system.grayscale);
  invert.check(config::system.invert);

  libfilter::colortable.set_contrast(config::system.contrast);
  libfilter::colortable.set_brightness(config::system.brightness);
  libfilter::colortable.set_gamma(config::system.gamma);
  libfilter::colortable.enable_gamma_ramp(config::system.gamma_ramp);
  libfilter::colortable.enable_sepia(config::system.sepia);
  libfilter::colortable.enable_grayscale(config::system.grayscale);
  libfilter::colortable.enable_invert(config::system.invert);
  libfilter::colortable.update();
}

uintptr_t VideoSettingsWindow::contrast_change(event_t) {
  if(config::system.contrast != contrast.get_position() - 95) {
    config::system.contrast = contrast.get_position() - 95;
    sync_ui();
  }
  return true;
}

uintptr_t VideoSettingsWindow::brightness_change(event_t) {
  if(config::system.brightness != brightness.get_position() - 95) {
    config::system.brightness = brightness.get_position() - 95;
    sync_ui();
  }
  return true;
}

uintptr_t VideoSettingsWindow::gamma_change(event_t) {
  if(config::system.gamma != gamma.get_position() + 5) {
    config::system.gamma = gamma.get_position() + 5;
    sync_ui();
  }
  return true;
}

uintptr_t VideoSettingsWindow::gammaramp_tick(event_t) {
  if(config::system.gamma_ramp != gamma_ramp.checked()) {
    config::system.gamma_ramp = gamma_ramp.checked();
    sync_ui();
  }
  return true;
}

uintptr_t VideoSettingsWindow::sepia_tick(event_t) {
  if(config::system.sepia != sepia.checked()) {
    config::system.sepia = sepia.checked();
    sync_ui();
  }
  return true;
}

uintptr_t VideoSettingsWindow::grayscale_tick(event_t) {
  if(config::system.grayscale != grayscale.checked()) {
    config::system.grayscale = grayscale.checked();
    sync_ui();
  }
  return true;
}

uintptr_t VideoSettingsWindow::invert_tick(event_t) {
  if(config::system.invert != invert.checked()) {
    config::system.invert = invert.checked();
    sync_ui();
  }
  return true;
}
