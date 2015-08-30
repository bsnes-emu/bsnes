AudioSettings::AudioSettings(TabFrame* parent) : TabFrameItem(parent) {
  setImage(Icon::Device::Speaker);
  setText("Audio");

  layout.setMargin(5);

  frequencyLabel.setText("Frequency:");
  frequencyCombo.append(ComboButtonItem().setText("32000hz"));
  frequencyCombo.append(ComboButtonItem().setText("44100hz"));
  frequencyCombo.append(ComboButtonItem().setText("48000hz"));
  frequencyCombo.append(ComboButtonItem().setText("96000hz"));
  switch(config->audio.frequency) {
  case 32000: frequencyCombo.item(0)->setSelected(); break;
  case 44100: frequencyCombo.item(1)->setSelected(); break;
  case 48000: frequencyCombo.item(2)->setSelected(); break;
  case 96000: frequencyCombo.item(3)->setSelected(); break;
  }
  frequencyCombo.onChange([&] { update(); });

  latencyLabel.setText("Latency:");
  latencyCombo.append(ComboButtonItem().setText("20ms"));
  latencyCombo.append(ComboButtonItem().setText("40ms"));
  latencyCombo.append(ComboButtonItem().setText("60ms"));
  latencyCombo.append(ComboButtonItem().setText("80ms"));
  latencyCombo.append(ComboButtonItem().setText("100ms"));
  switch(config->audio.latency) {
  case  20: latencyCombo.item(0)->setSelected(); break;
  case  40: latencyCombo.item(1)->setSelected(); break;
  case  60: latencyCombo.item(2)->setSelected(); break;
  case  80: latencyCombo.item(3)->setSelected(); break;
  case 100: latencyCombo.item(4)->setSelected(); break;
  }
  latencyCombo.onChange([&] { update(); });

  resamplerLabel.setText("Resampler:");
  resamplerCombo.append(ComboButtonItem().setText("Linear"));
  resamplerCombo.append(ComboButtonItem().setText("Hermite"));
  resamplerCombo.append(ComboButtonItem().setText("Sinc"));
  if(config->audio.resampler == "Linear" ) resamplerCombo.item(0)->setSelected();
  if(config->audio.resampler == "Hermite") resamplerCombo.item(1)->setSelected();
  if(config->audio.resampler == "Sinc"   ) resamplerCombo.item(2)->setSelected();
  resamplerCombo.onChange([&] { update(); });

  volumeLabel.setText("Volume:");
  volumeSlider.setLength(201).setPosition(config->audio.volume).onChange([&] { updateVolume(); });

  update();
}

auto AudioSettings::update() -> void {
  if(auto item = frequencyCombo.selected()) {
    if(item->offset() == 0) config->audio.frequency = 32000;
    if(item->offset() == 1) config->audio.frequency = 44100;
    if(item->offset() == 2) config->audio.frequency = 48000;
    if(item->offset() == 3) config->audio.frequency = 96000;
  }
  if(auto item = latencyCombo.selected()) {
    if(item->offset() == 0) config->audio.latency =  20;
    if(item->offset() == 1) config->audio.latency =  40;
    if(item->offset() == 2) config->audio.latency =  60;
    if(item->offset() == 3) config->audio.latency =  80;
    if(item->offset() == 4) config->audio.latency = 100;
  }
  if(auto item = resamplerCombo.selected()) {
    if(item->offset() == 0) config->audio.resampler = "Linear";
    if(item->offset() == 1) config->audio.resampler = "Hermite";
    if(item->offset() == 2) config->audio.resampler = "Sinc";
  }
  updateVolume();
  program->updateAudio();
}

auto AudioSettings::updateVolume() -> void {
  config->audio.volume = volumeSlider.position();
  volumeValue.setText({config->audio.volume, "%"});
  program->dsp.setVolume(config->audio.mute ? 0.0 : config->audio.volume * 0.01);
}
