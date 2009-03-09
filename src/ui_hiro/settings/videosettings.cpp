void VideoSettingsWindow::setup() {
  create(0, 451, 370);

  lcontrast.create   (0, 451, 18);
  contrast.create    (0, 451, 30, 191);
  lbrightness.create (0, 451, 18);
  brightness.create  (0, 451, 30, 191);
  lgamma.create      (0, 451, 18);
  gamma.create       (0, 451, 30, 191);
  gamma_ramp.create  (0, 223, 18, translate["Simulate TV gamma ramp"]);
  merge_fields.create(0, 223, 18, translate["Merge fields for NTSC filter"]);

  unsigned y = 0;
  attach(lcontrast,   0, y); y += 18;
  attach(contrast,    0, y); y += 30;
  attach(lbrightness, 0, y); y += 18;
  attach(brightness,  0, y); y += 30;
  attach(lgamma,      0, y); y += 18;
  attach(gamma,       0, y); y += 30;

  attach(gamma_ramp,     0, y);
  attach(merge_fields, 228, y); y += 18;

  contrast.on_change   = bind(&VideoSettingsWindow::contrast_change, this);
  brightness.on_change = bind(&VideoSettingsWindow::brightness_change, this);
  gamma.on_change      = bind(&VideoSettingsWindow::gamma_change, this);
  gamma_ramp.on_tick   = bind(&VideoSettingsWindow::gammaramp_tick, this);
  merge_fields.on_tick = bind(&VideoSettingsWindow::merge_fields_tick, this);

  sync_ui();
}

//update all UI controls to match config file values ...
void VideoSettingsWindow::sync_ui() {
  contrast.set_position(config.video.contrast + 95);
  lcontrast.set_text(string()
  << translate["Contrast adjust"] << ": "
  << ((int)config.video.contrast > 0 ? "+" : "")
  << (int)config.video.contrast << "%");
  brightness.set_position(config.video.brightness + 95);
  lbrightness.set_text(string()
  << translate["Brightness adjust"] << ": "
  << ((int)config.video.brightness > 0 ? "+" : "")
  << (int)config.video.brightness << "%");
  gamma.set_position(config.video.gamma - 5);
  lgamma.set_text(string()
  << translate["Gamma adjust"] << ": "
  << ((int)config.video.gamma > 100 ? "+" : "")
  << (int)(config.video.gamma - 100) << "%");

  gamma_ramp.check(config.video.gamma_ramp);
  merge_fields.check(config.video.ntsc_filter_merge_fields);

  libfilter::colortable.set_contrast(config.video.contrast);
  libfilter::colortable.set_brightness(config.video.brightness);
  libfilter::colortable.set_gamma(config.video.gamma);
  libfilter::colortable.enable_gamma_ramp(config.video.gamma_ramp);
  libfilter::colortable.enable_sepia(config.video.sepia);
  libfilter::colortable.enable_grayscale(config.video.grayscale);
  libfilter::colortable.enable_invert(config.video.invert);
  libfilter::colortable.update();
}

uintptr_t VideoSettingsWindow::contrast_change(event_t) {
  if(config.video.contrast != contrast.get_position() - 95) {
    config.video.contrast = contrast.get_position() - 95;
    sync_ui();
  }
  return true;
}

uintptr_t VideoSettingsWindow::brightness_change(event_t) {
  if(config.video.brightness != brightness.get_position() - 95) {
    config.video.brightness = brightness.get_position() - 95;
    sync_ui();
  }
  return true;
}

uintptr_t VideoSettingsWindow::gamma_change(event_t) {
  if(config.video.gamma != gamma.get_position() + 5) {
    config.video.gamma = gamma.get_position() + 5;
    sync_ui();
  }
  return true;
}

uintptr_t VideoSettingsWindow::gammaramp_tick(event_t) {
  if(config.video.gamma_ramp != gamma_ramp.checked()) {
    config.video.gamma_ramp = gamma_ramp.checked();
    sync_ui();
  }
  return true;
}

uintptr_t VideoSettingsWindow::merge_fields_tick(event_t) {
  if(config.video.ntsc_filter_merge_fields != merge_fields.checked()) {
    config.video.ntsc_filter_merge_fields = merge_fields.checked();
    libfilter::filter_ntsc.adjust(0, 0, 0, 0, 0, config.video.ntsc_filter_merge_fields);
    sync_ui();
  }
  return true;
}
