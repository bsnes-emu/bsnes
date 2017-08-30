AudioSettings::AudioSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Speaker);
  setText("Audio");

  layout.setMargin(5);

  driverLabel.setFont(Font().setBold()).setText("Driver");

  deviceLabel.setText("Device:");
  deviceList.onChange([&] {
    settings["Audio/Device"].setValue(deviceList.selected().text());
    program->initializeAudioDriver();
    updateDevice();
  });

  for(auto& device : audio->availableDevices()) {
    deviceList.append(ComboButtonItem().setText(device));
    if(device == settings["Audio/Device"].text()) {
      deviceList.item(deviceList.itemCount() - 1).setSelected();
    }
  }

  frequencyLabel.setText("Frequency:");
  frequencyList.onChange([&] {
    settings["Audio/Frequency"].setValue(frequencyList.selected().text());
    program->updateAudioDriver();
  });

  latencyLabel.setText("Latency:");
  latencyList.onChange([&] {
    settings["Audio/Latency"].setValue(latencyList.selected().text());
    program->updateAudioDriver();
  });

  exclusiveMode.setText("Exclusive mode");
  exclusiveMode.setChecked(settings["Audio/Exclusive"].boolean()).onToggle([&] {
    settings["Audio/Exclusive"].setValue(exclusiveMode.checked());
    program->updateAudioDriver();
  });

  effectsLabel.setFont(Font().setBold()).setText("Effects");

  volumeLabel.setText("Volume:");
  volumeValue.setAlignment(0.5);
  volumeSlider.setLength(501).setPosition(settings["Audio/Volume"].natural()).onChange([&] { updateEffects(); });

  balanceLabel.setText("Balance:");
  balanceValue.setAlignment(0.5);
  balanceSlider.setLength(101).setPosition(settings["Audio/Balance"].natural()).onChange([&] { updateEffects(); });

  reverbEnable.setText("Reverb").setChecked(settings["Audio/Reverb/Enable"].boolean()).onToggle([&] { updateEffects(); });

  updateDevice();
  updateEffects(true);
}

auto AudioSettings::updateDevice() -> void {
  frequencyList.reset();
  for(auto& frequency : audio->availableFrequencies()) {
    frequencyList.append(ComboButtonItem().setText((uint)frequency));
    if(frequency == settings["Audio/Frequency"].real()) {
      frequencyList.item(frequencyList.itemCount() - 1).setSelected();
    }
  }

  latencyList.reset();
  for(auto& latency : audio->availableLatencies()) {
    latencyList.append(ComboButtonItem().setText(latency));
    if(latency == settings["Audio/Latency"].natural()) {
      latencyList.item(latencyList.itemCount() - 1).setSelected();
    }
  }
}

auto AudioSettings::updateEffects(bool initializing) -> void {
  settings["Audio/Volume"].setValue(volumeSlider.position());
  volumeValue.setText({volumeSlider.position(), "%"});

  settings["Audio/Balance"].setValue(balanceSlider.position());
  balanceValue.setText({balanceSlider.position(), "%"});

  settings["Audio/Reverb/Enable"].setValue(reverbEnable.checked());

  if(!initializing) program->updateAudioEffects();
}
