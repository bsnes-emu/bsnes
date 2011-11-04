AudioSettings *audioSettings = 0;

AudioSlider::AudioSlider() {
  append(name,   { 75, 0 });
  append(value,  { 75, 0 });
  append(slider, { ~0, 0 });
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

  outputLabel.setFont(application->boldFont);
  outputLabel.setText("Output settings:");

  frequencyLabel.setText("Frequency:");
  frequencySelection.append("32000hz");
  frequencySelection.append("44100hz");
  frequencySelection.append("48000hz");
  frequencySelection.append("96000hz");

  latencyLabel.setText("Latency:");
  latencySelection.append( "40ms");
  latencySelection.append( "60ms");
  latencySelection.append( "80ms");
  latencySelection.append("100ms");
  latencySelection.append("120ms");

  resamplerLabel.setText("Resampler:");
  resamplerSelection.append("Linear");
  resamplerSelection.append("Hermite");
  resamplerSelection.append("Sinc");

  volume.name.setText("Volume:");
  volume.slider.setLength(201);
  volume.base = 100;
  volume.step = 1;

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

  append(title,                           { ~0, 0 }, 5);
  append(outputLabel,                     { ~0, 0 }, 0);
  append(outputLayout,                    { ~0, 0 }, 5);
  outputLayout.append(frequencyLabel,     {  0, 0 }, 5);
  outputLayout.append(frequencySelection, { ~0, 0 }, 5);
  outputLayout.append(latencyLabel,       {  0, 0 }, 5);
  outputLayout.append(latencySelection,   { ~0, 0 }, 5);
  outputLayout.append(resamplerLabel,     {  0, 0 }, 5);
  outputLayout.append(resamplerSelection, { ~0, 0 }, 0);
  append(volume,                          { ~0, 0 }, 5);
  append(frequencyAdjustmentLabel,        { ~0, 0 }, 0);
  append(nes,                             { ~0, 0 }, 0);
  append(snes,                            { ~0, 0 }, 0);
  append(gameBoy,                         { ~0, 0 }, 0);

  frequencySelection.setSelection(
    config->audio.frequency == 32000 ? 0 :
    config->audio.frequency == 44100 ? 1 :
    config->audio.frequency == 48000 ? 2 :
    config->audio.frequency == 96000 ? 3 : 0
  );

  latencySelection.setSelection(
    config->audio.latency ==  40 ? 0 :
    config->audio.latency ==  60 ? 1 :
    config->audio.latency ==  80 ? 2 :
    config->audio.latency == 100 ? 3 :
    config->audio.latency == 120 ? 4 : 0
  );

  resamplerSelection.setSelection(
    config->audio.resampler == "linear"  ? 0 :
    config->audio.resampler == "hermite" ? 1 :
    config->audio.resampler == "sinc"    ? 2 : 0
  );

  volume.setPosition(config->audio.volume);

  nes.setPosition(config->audio.frequencyNES);
  snes.setPosition(config->audio.frequencySNES);
  gameBoy.setPosition(config->audio.frequencyGameBoy);

  frequencySelection.onChange = latencySelection.onChange = resamplerSelection.onChange =
  volume.slider.onChange = nes.slider.onChange = snes.slider.onChange = gameBoy.slider.onChange =
  { &AudioSettings::synchronize, this };

  synchronize();
}

void AudioSettings::synchronize() {
  config->audio.frequency =
  frequencySelection.selection() == 0 ? 32000 :
  frequencySelection.selection() == 1 ? 44100 :
  frequencySelection.selection() == 2 ? 48000 :
  frequencySelection.selection() == 3 ? 96000 : 48000;

  config->audio.latency =
  latencySelection.selection() == 0 ?  40 :
  latencySelection.selection() == 1 ?  60 :
  latencySelection.selection() == 2 ?  80 :
  latencySelection.selection() == 3 ? 100 :
  latencySelection.selection() == 4 ? 120 : 60;

  config->audio.resampler =
  resamplerSelection.selection() == 0 ? "linear" :
  resamplerSelection.selection() == 1 ? "hermite" :
  resamplerSelection.selection() == 2 ? "sinc" : "sinc";

  config->audio.volume = volume.position();

  config->audio.frequencyNES = nes.position();
  config->audio.frequencySNES = snes.position();
  config->audio.frequencyGameBoy = gameBoy.position();

  nes.value.setText({ nes.position(), "hz" });
  snes.value.setText({ snes.position(), "hz" });
  gameBoy.value.setText({ gameBoy.position(), "hz" });
  volume.value.setText({ volume.position(), "%" });

  interface->updateDSP();
}
