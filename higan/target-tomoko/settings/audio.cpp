AudioSettings::AudioSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Speaker);
  setText("Audio");

  layout.setMargin(5);

  driverLabel.setFont(Font().setBold()).setText("Driver Settings");

  auto information = audio->information();

  deviceLabel.setText("Device:");
  for(auto& device : information.devices) {
    deviceList.append(ComboButtonItem().setText(device));
    if(device == settings["Audio/Device"].text()) {
      deviceList.item(deviceList.itemCount() - 1).setSelected();
    }
  }
  deviceList.onChange([&] { updateDriver(); });

  frequencyLabel.setText("Frequency:");
  for(auto& frequency : information.frequencies) {
    frequencyList.append(ComboButtonItem().setText(frequency));
    if(frequency == settings["Audio/Frequency"].natural()) {
      frequencyList.item(frequencyList.itemCount() - 1).setSelected();
    }
  }
  frequencyList.onChange([&] { updateDriver(); });

  latencyLabel.setText("Latency:");
  for(auto& latency : information.latencies) {
    latencyList.append(ComboButtonItem().setText(latency));
    if(latency == settings["Audio/Latency"].natural()) {
      latencyList.item(latencyList.itemCount() - 1).setSelected();
    }
  }
  latencyList.onChange([&] { updateDriver(); });

  exclusiveMode.setText("Exclusive mode");
  exclusiveMode.setChecked(settings["Audio/Exclusive"].boolean()).onToggle([&] { updateDriver(); });

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
  settings["Audio/Device"].setValue(deviceList.selected().text());
  settings["Audio/Frequency"].setValue(frequencyList.selected().text());
  settings["Audio/Latency"].setValue(latencyList.selected().text());
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
