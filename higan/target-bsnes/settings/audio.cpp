AudioSettings::AudioSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Speaker);
  setText("Audio");

  layout.setMargin(5);

  driverLabel.setFont(Font().setBold()).setText("Driver");
  deviceLabel.setText("Device:");
  deviceList.onChange([&] {
    settings["Audio/Device"].setValue(deviceList.selected().text());
    program->updateAudioDevice();
  });
  frequencyLabel.setText("Frequency:");
  frequencyList.onChange([&] {
    settings["Audio/Frequency"].setValue(frequencyList.selected().text());
    program->updateAudioFrequency();
  });
  latencyLabel.setText("Latency:");
  latencyList.onChange([&] {
    settings["Audio/Latency"].setValue(latencyList.selected().text());
    program->updateAudioLatency();
  });
  exclusiveMode.setText("Exclusive mode").setChecked(settings["Audio/Exclusive"].boolean()).onToggle([&] {
    settings["Audio/Exclusive"].setValue(exclusiveMode.checked());
    program->updateAudioExclusive();
  });

  effectsLabel.setFont(Font().setBold()).setText("Effects");
  skewLabel.setAlignment(1.0).setText("Skew:");
  skewValue.setAlignment(0.5);
  skewSlider.setLength(10001).setPosition(settings["Audio/Skew"].integer() + 5000).onChange([&] {
    string value = {skewSlider.position() > 5000 ? "+" : "", (int)skewSlider.position() - 5000};
    settings["Audio/Skew"].setValue(value);
    skewValue.setText(value);
    program->updateAudioFrequency();
  }).doChange();
  volumeLabel.setAlignment(1.0).setText("Volume:");
  volumeValue.setAlignment(0.5);
  volumeSlider.setLength(201).setPosition(settings["Audio/Volume"].natural()).onChange([&] {
    string value = {volumeSlider.position(), "%"};
    settings["Audio/Volume"].setValue(value);
    volumeValue.setText(value);
    program->updateAudioEffects();
  }).doChange();
  balanceLabel.setAlignment(1.0).setText("Balance:");
  balanceValue.setAlignment(0.5);
  balanceSlider.setLength(101).setPosition(settings["Audio/Balance"].natural()).onChange([&] {
    string value = {balanceSlider.position(), "%"};
    settings["Audio/Balance"].setValue(value);
    balanceValue.setText(value);
    program->updateAudioEffects();
  }).doChange();
  reverb.setText("Reverb").setChecked(settings["Audio/Reverb"].boolean()).onToggle([&] {
    settings["Audio/Reverb"].setValue(reverb.checked());
    program->updateAudioEffects();
  });
  //todo: does not work properly with Super Game Boy
  reverb.setVisible(false);
}

auto AudioSettings::updateDevice() -> void {
  deviceList.reset();
  for(auto& device : audio->availableDevices()) {
    deviceList.append(ComboButtonItem().setText(device));
    if(device == settings["Audio/Device"].text()) {
      deviceList.items().right().setSelected();
    }
  }
}

auto AudioSettings::updateFrequency() -> void {
  frequencyList.reset();
  for(auto& frequency : audio->availableFrequencies()) {
    frequencyList.append(ComboButtonItem().setText((uint)frequency));
    if(frequency == settings["Audio/Frequency"].real()) {
      frequencyList.items().right().setSelected();
    }
  }
}

auto AudioSettings::updateLatency() -> void {
  latencyList.reset();
  for(auto& latency : audio->availableLatencies()) {
    latencyList.append(ComboButtonItem().setText(latency));
    if(latency == settings["Audio/Latency"].natural()) {
      latencyList.items().right().setSelected();
    }
  }
}
