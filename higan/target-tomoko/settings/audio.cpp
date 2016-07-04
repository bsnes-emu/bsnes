AudioSettings::AudioSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Speaker);
  setText("Audio");

  layout.setMargin(5);

  driverLabel.setFont(Font().setBold()).setText("Driver Settings");

  latencyLabel.setText("Latency:");
  latencyCombo.append(ComboButtonItem().setText("0ms"));
  latencyCombo.append(ComboButtonItem().setText("20ms"));
  latencyCombo.append(ComboButtonItem().setText("40ms"));
  latencyCombo.append(ComboButtonItem().setText("60ms"));
  latencyCombo.append(ComboButtonItem().setText("80ms"));
  latencyCombo.append(ComboButtonItem().setText("100ms"));
  switch(settings["Audio/Latency"].natural()) {
  case   0: latencyCombo.item(0)->setSelected(); break;
  case  20: latencyCombo.item(1)->setSelected(); break;
  case  40: latencyCombo.item(2)->setSelected(); break;
  case  60: latencyCombo.item(3)->setSelected(); break;
  case  80: latencyCombo.item(4)->setSelected(); break;
  case 100: latencyCombo.item(5)->setSelected(); break;
  }
  latencyCombo.onChange([&] { updateDriver(); });

  frequencyLabel.setText("Frequency:");
  auto frequencyValue = audio->get(Audio::Frequency).get<uint>(44100);
  frequencyCombo.append(ComboButtonItem().setText({frequencyValue, "hz"}));
  frequencyCombo.setEnabled(false);

  resamplerLabel.setText("Resampler:");
  resamplerCombo.append(ComboButtonItem().setText("IIR - Biquad"));
  resamplerCombo.setEnabled(false);

  exclusiveMode.setText("Exclusive Mode");
  exclusiveMode.setChecked(settings["Audio/Exclusive"].boolean()).onToggle([&] { updateDriver(); });
  if(!audio->cap(Audio::Exclusive)) exclusiveMode.remove();

  effectsLabel.setFont(Font().setBold()).setText("Effects");

  volumeLabel.setText("Volume:");
  volumeValue.setAlignment(0.5);
  volumeSlider.setLength(501).setPosition(settings["Audio/Volume"].natural()).onChange([&] { updateEffects(); });

  balanceLabel.setText("Balance:");
  balanceValue.setAlignment(0.5);
  balanceSlider.setLength(101).setPosition(settings["Audio/Balance"].natural()).onChange([&] { updateEffects(); });

  reverbEnable.setText("Reverb").setChecked(settings["Audio/Reverb/Enable"].boolean()).onToggle([&] { updateEffects(); });

  updateDriver();
  updateEffects();
}

auto AudioSettings::updateDriver() -> void {
  if(auto item = latencyCombo.selected()) {
    uint latency = 60;
    if(item->offset() == 0) latency =   0;
    if(item->offset() == 1) latency =  20;
    if(item->offset() == 2) latency =  40;
    if(item->offset() == 3) latency =  60;
    if(item->offset() == 4) latency =  80;
    if(item->offset() == 5) latency = 100;
    settings["Audio/Latency"].setValue(latency);
  }

  settings["Audio/Exclusive"].setValue(exclusiveMode.checked());
  program->updateAudioDriver();
}

auto AudioSettings::updateEffects() -> void {
  settings["Audio/Volume"].setValue(volumeSlider.position());
  volumeValue.setText({volumeSlider.position(), "%"});

  settings["Audio/Balance"].setValue(balanceSlider.position());
  balanceValue.setText({balanceSlider.position(), "%"});

  settings["Audio/Reverb/Enable"].setValue(reverbEnable.checked());

  program->updateAudioEffects();
}
