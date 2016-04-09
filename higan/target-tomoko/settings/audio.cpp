AudioSettings::AudioSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Speaker);
  setText("Audio");

  layout.setMargin(5);

  frequencyLabel.setText("Frequency:");
  auto frequencyValue = audio->get(Audio::Frequency).get<uint>();
  frequencyCombo.append(ComboButtonItem().setText({frequencyValue, "hz"}));
  frequencyCombo.setEnabled(false);  //not user configurable

  latencyLabel.setText("Latency:");
  latencyCombo.append(ComboButtonItem().setText("20ms"));
  latencyCombo.append(ComboButtonItem().setText("40ms"));
  latencyCombo.append(ComboButtonItem().setText("60ms"));
  latencyCombo.append(ComboButtonItem().setText("80ms"));
  latencyCombo.append(ComboButtonItem().setText("100ms"));
  switch(settings["Audio/Latency"].natural()) {
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
  if(settings["Audio/Resampler"].text() == "Linear" ) resamplerCombo.item(0)->setSelected();
  if(settings["Audio/Resampler"].text() == "Hermite") resamplerCombo.item(1)->setSelected();
  if(settings["Audio/Resampler"].text() == "Sinc"   ) resamplerCombo.item(2)->setSelected();
  resamplerCombo.onChange([&] { update(); });

  volumeLabel.setText("Volume:");
  volumeSlider.setLength(201).setPosition(settings["Audio/Volume"].natural()).onChange([&] { updateVolume(); });

  update();
}

auto AudioSettings::update() -> void {
  if(auto item = latencyCombo.selected()) {
    uint latency = 60;
    if(item->offset() == 0) latency =  20;
    if(item->offset() == 1) latency =  40;
    if(item->offset() == 2) latency =  60;
    if(item->offset() == 3) latency =  80;
    if(item->offset() == 4) latency = 100;
    settings["Audio/Latency"].setValue(latency);
  }
  if(auto item = resamplerCombo.selected()) {
    string resampler = "Sinc";
    if(item->offset() == 0) resampler = "Linear";
    if(item->offset() == 1) resampler = "Hermite";
    if(item->offset() == 2) resampler = "Sinc";
    settings["Audio/Resampler"].setValue(resampler);
  }
  updateVolume();
  program->updateAudio();
}

auto AudioSettings::updateVolume() -> void {
  settings["Audio/Volume"].setValue(volumeSlider.position());
  volumeValue.setText({volumeSlider.position(), "%"});
  program->updateAudioVolume();
}
