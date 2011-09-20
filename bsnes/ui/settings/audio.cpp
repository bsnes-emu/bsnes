AudioSettings *audioSettings = 0;

AudioSlider::AudioSlider() {
  append(name, 75, 0);
  append(value, 75, 0);
  append(slider, ~0, 0);
}

unsigned AudioSlider::position() {
  unsigned value = slider.position(), center = slider.length() >> 1;
  if(value > center) return base + (value - center) * step;
  if(value < center) return base - (center - value) * step;
  return base;
}

void AudioSlider::setPosition(unsigned position) {
  signed value = position - base, center = slider.length() >> 1;
  if(value < 0) return slider.setPosition(center - (abs(value) / step));
  if(value > 0) return slider.setPosition((abs(value) / step) + center);
  return slider.setPosition(center);
}

AudioSettings::AudioSettings() {
  title.setFont(application->titleFont);
  title.setText("Audio Settings");
  frequencyAdjustmentLabel.setFont(application->boldFont);
  frequencyAdjustmentLabel.setText("Frequency: (lower to reduce audio crackling; raise to reduce video tearing)");

  nes.name.setText("NES:");
  nes.slider.setLength(2001);
  nes.base = 1789772;
  nes.step = 56;

  snes.name.setText("SNES:");
  snes.slider.setLength(2001);
  snes.base = 32000;
  snes.step = 1;

  gameBoy.name.setText("Game Boy:");
  gameBoy.slider.setLength(2001);
  gameBoy.base = 4194304;
  gameBoy.step = 131;

  outputAdjustmentLabel.setFont(application->boldFont);
  outputAdjustmentLabel.setText("Output:");

  volume.name.setText("Volume:");
  volume.slider.setLength(201);
  volume.base = 100;
  volume.step = 1;

  append(title, ~0, 0, 5);
  append(frequencyAdjustmentLabel, ~0, 0);
  append(nes, ~0, 0);
  append(snes, ~0, 0);
  append(gameBoy, ~0, 0, 5);
  append(outputAdjustmentLabel, ~0, 0);
  append(volume, ~0, 0);

  nes.setPosition(config->audio.frequencyNES);
  snes.setPosition(config->audio.frequencySNES);
  gameBoy.setPosition(config->audio.frequencyGameBoy);
  volume.setPosition(config->audio.volume);

  nes.slider.onChange = snes.slider.onChange = gameBoy.slider.onChange =
  volume.slider.onChange =
  { &AudioSettings::synchronize, this };

  synchronize();
}

void AudioSettings::synchronize() {
  config->audio.frequencyNES = nes.position();
  config->audio.frequencySNES = snes.position();
  config->audio.frequencyGameBoy = gameBoy.position();
  config->audio.volume = volume.position();

  nes.value.setText({ nes.position(), "hz" });
  snes.value.setText({ snes.position(), "hz" });
  gameBoy.value.setText({ gameBoy.position(), "hz" });
  volume.value.setText({ volume.position(), "%" });

  interface->updateDSP();
}
