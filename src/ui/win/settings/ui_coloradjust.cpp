bool ColorAdjustWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_CHANGED: {
    if(info.control == &ContrastSlider) {
      config::snes.contrast = ContrastSlider.GetPos();
      ContrastLabel.SetText("Contrast: %d", int32(config::snes.contrast));
      snes->update_color_lookup_table();
    } else if(info.control == &BrightnessSlider) {
      config::snes.brightness = BrightnessSlider.GetPos();
      BrightnessLabel.SetText("Brightness: %d", int32(config::snes.brightness));
      snes->update_color_lookup_table();
    } else if(info.control == &GammaSlider) {
      config::snes.gamma = GammaSlider.GetPos();
      GammaLabel.SetText("Gamma: %0.2f", double(config::snes.gamma) / 100.0);
      snes->update_color_lookup_table();
    }
  } break;

  case EVENT_CLICKED: {
    if(info.control == &GammaRamp) {
      config::snes.gamma_ramp.toggle();
      GammaRamp.Check(config::snes.gamma_ramp);
      snes->update_color_lookup_table();
    } else if(info.control == &Sepia) {
      config::snes.sepia.toggle();
      Sepia.Check(config::snes.sepia);
      snes->update_color_lookup_table();
    } else if(info.control == &Grayscale) {
      config::snes.grayscale.toggle();
      Grayscale.Check(config::snes.grayscale);
      snes->update_color_lookup_table();
    } else if(info.control == &Invert) {
      config::snes.invert.toggle();
      Invert.Check(config::snes.invert);
      snes->update_color_lookup_table();
    } else if(info.control == &Restore) {
      config::snes.contrast   =   0;
      config::snes.brightness =   0;
      config::snes.gamma      =  80;
      config::snes.gamma_ramp = true;
      config::snes.sepia      = false;
      config::snes.grayscale  = false;
      config::snes.invert     = false;
      ContrastSlider.SetPos(config::snes.contrast);
      BrightnessSlider.SetPos(config::snes.brightness);
      GammaSlider.SetPos(config::snes.gamma);
      ContrastLabel.SetText("Contrast: %d", int32(config::snes.contrast));
      BrightnessLabel.SetText("Brightness: %d", int32(config::snes.brightness));
      GammaLabel.SetText("Gamma: %0.2f", double(config::snes.gamma) / 100.0);
      GammaRamp.Check(config::snes.gamma_ramp);
      Sepia.Check(config::snes.sepia);
      Grayscale.Check(config::snes.grayscale);
      Invert.Check(config::snes.invert);
      snes->update_color_lookup_table();
    }
  } break;

  }

  return false;
}

void ColorAdjustWindow::Setup() {
  Header.Create(this, "visible", 0, 0, 475, 30, "Color Adjustment");
  Header.SetFont(global::font_header);

int x = 15, y = 30;
  ContrastLabel.Create(this, "visible", x, y, 100, 15);
  ContrastLabel.SetText("Contrast: %d", int32(config::snes.contrast));
  ContrastSlider.Create(this, "visible", x + 105, y - 3, 355, 25);
  ContrastSlider.SetRange(-96, 95);
  ContrastSlider.SetPos(int32(config::snes.contrast));

  BrightnessLabel.Create(this, "visible", x, y + 25, 100, 15);
  BrightnessLabel.SetText("Brightness: %d", int32(config::snes.brightness));
  BrightnessSlider.Create(this, "visible", x + 105, y - 3 + 25, 355, 25);
  BrightnessSlider.SetRange(-96, 95);
  BrightnessSlider.SetPos(int32(config::snes.brightness));

  GammaLabel.Create(this, "visible", x, y + 50, 100, 15);
  GammaLabel.SetText("Gamma: %0.2f", double(config::snes.gamma) / 100.0);
  GammaSlider.Create(this, "visible", x + 105, y - 3 + 50, 355, 25);
  GammaSlider.SetRange(10, 500);
  GammaSlider.SetPos(int32(config::snes.gamma));
  y += 75;

  GammaRamp.Create(this, "visible", x, y, 475, 15, "Half gamma adjust (darkens image, helps prevent washed out colors)");
  GammaRamp.Check(config::snes.gamma_ramp);
  y += 15;
  Sepia.Create(this, "visible", x, y, 475, 15, "Sepia");
  Sepia.Check(config::snes.sepia);
  y += 15;
  Grayscale.Create(this, "visible", x, y, 475, 15, "Grayscale");
  Grayscale.Check(config::snes.grayscale);
  y += 15;
  Invert.Create(this, "visible", x, y, 475, 15, "Invert colors");
  Invert.Check(config::snes.invert);
  y += 20;

  Restore.Create(this, "visible", x, y, 120, 25, "Restore Defaults");
}
