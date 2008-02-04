void RasterSettingsWindow::setup() {
  create(0, 475, 355);

  lcontrast.create      (0, 475, 20);
  contrast.create       (0, 475, 30, 192);
  lbrightness.create    (0, 475, 20);
  brightness.create     (0, 475, 30, 192);
  lgamma.create         (0, 475, 20);
  gamma.create          (0, 475, 30, 191);
  gamma_ramp.create     (0, 235, 20, "Gamma ramp");
  sepia.create          (0, 235, 20, "Sepia");
  grayscale.create      (0, 235, 20, "Grayscale");
  invert.create         (0, 235, 20, "Invert colors");
  preset_optimal.create (0, 235, 30, "Optimal Preset");
  preset_standard.create(0, 235, 30, "Standard Preset");
  sync_ui();

uint y = 0;
  attach(lcontrast,         0, y); y += 20;
  attach(contrast,          0, y); y += 30;
  attach(lbrightness,       0, y); y += 20;
  attach(brightness,        0, y); y += 30;
  attach(lgamma,            0, y); y += 20;
  attach(gamma,             0, y); y += 30;
  attach(gamma_ramp,        0, y);
  attach(sepia,           240, y); y += 20;
  attach(grayscale,         0, y);
  attach(invert,          240, y); y += 20;
  attach(preset_optimal,    0, y);
  attach(preset_standard, 240, y); y += 30;

  contrast.on_change      = bind(&RasterSettingsWindow::contrast_change, this);
  brightness.on_change    = bind(&RasterSettingsWindow::brightness_change, this);
  gamma.on_change         = bind(&RasterSettingsWindow::gamma_change, this);
  gamma_ramp.on_tick      = bind(&RasterSettingsWindow::gammaramp_tick, this);
  sepia.on_tick           = bind(&RasterSettingsWindow::sepia_tick, this);
  grayscale.on_tick       = bind(&RasterSettingsWindow::grayscale_tick, this);
  invert.on_tick          = bind(&RasterSettingsWindow::invert_tick, this);
  preset_optimal.on_tick  = bind(&RasterSettingsWindow::optimal_tick, this);
  preset_standard.on_tick = bind(&RasterSettingsWindow::standard_tick, this);
}

//update all UI controls to match config file values ...
void RasterSettingsWindow::sync_ui() {
  ui_lock = true; //supress event messages while syncing UI elements, prevents infinite recursion
  contrast.set_position(config::snes.contrast + 96);
  lcontrast.set_text(string() << "Contrast: " << (int)config::snes.contrast);
  brightness.set_position(config::snes.brightness + 96);
  lbrightness.set_text(string() << "Brightness: " << (int)config::snes.brightness);
  gamma.set_position(config::snes.gamma - 10);
  lgamma.set_text(string() << "Gamma: " << (int)config::snes.gamma); //TODO: print gamma as "%0.2f" / 100.0
  gamma_ramp.check(config::snes.gamma_ramp);
  sepia.check(config::snes.sepia);
  grayscale.check(config::snes.grayscale);
  invert.check(config::snes.invert);
  snes.update_color_lookup_table();
  ui_lock = false;
}

uintptr_t RasterSettingsWindow::contrast_change(Event) {
  if(!ui_lock && config::snes.contrast != contrast.get_position() - 96) {
    config::snes.contrast = contrast.get_position() - 96;
    sync_ui();
  }
  return true;
}

uintptr_t RasterSettingsWindow::brightness_change(Event) {
  if(!ui_lock && config::snes.brightness != brightness.get_position() - 96) {
    config::snes.brightness = brightness.get_position() - 96;
    sync_ui();
  }
  return true;
}

uintptr_t RasterSettingsWindow::gamma_change(Event) {
  if(!ui_lock && config::snes.gamma != gamma.get_position() + 10) {
    config::snes.gamma = gamma.get_position() + 10;
    sync_ui();
  }
  return true;
}

uintptr_t RasterSettingsWindow::gammaramp_tick(Event) {
  if(!ui_lock && config::snes.gamma_ramp != gamma_ramp.checked()) {
    config::snes.gamma_ramp = gamma_ramp.checked();
    sync_ui();
  }
  return true;
}

uintptr_t RasterSettingsWindow::sepia_tick(Event) {
  if(!ui_lock && config::snes.sepia != sepia.checked()) {
    config::snes.sepia = sepia.checked();
    sync_ui();
  }
  return true;
}

uintptr_t RasterSettingsWindow::grayscale_tick(Event) {
  if(!ui_lock && config::snes.grayscale != grayscale.checked()) {
    config::snes.grayscale = grayscale.checked();
    sync_ui();
  }
  return true;
}

uintptr_t RasterSettingsWindow::invert_tick(Event) {
  if(!ui_lock && config::snes.invert != invert.checked()) {
    config::snes.invert = invert.checked();
    sync_ui();
  }
  return true;
}

uintptr_t RasterSettingsWindow::optimal_tick(Event) {
  config::snes.contrast   = 0;
  config::snes.brightness = 0;
  config::snes.gamma      = 80;
  config::snes.gamma_ramp = true;
  config::snes.sepia      = false;
  config::snes.grayscale  = false;
  config::snes.invert     = false;
  sync_ui();
  return true;
}

uintptr_t RasterSettingsWindow::standard_tick(Event) {
  config::snes.contrast   = 0;
  config::snes.brightness = 0;
  config::snes.gamma      = 100;
  config::snes.gamma_ramp = false;
  config::snes.sepia      = false;
  config::snes.grayscale  = false;
  config::snes.invert     = false;
  sync_ui();
  return true;
}

RasterSettingsWindow::RasterSettingsWindow() {
  ui_lock = false;
}
