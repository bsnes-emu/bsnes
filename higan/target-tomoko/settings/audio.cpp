AudioSettings::AudioSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Speaker);
  setText("Audio");

  layout.setMargin(5);

  driverLabel.setFont(Font().setBold()).setText("Driver Settings");

  deviceLabel.setText("Device:");
  deviceList.onChange([&] { updateDriver(); updateDriverLists(); });

  frequencyLabel.setText("Frequency:");
  frequencyList.onChange([&] { updateDriver(); });

  latencyLabel.setText("Latency:");
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

  updateDriverLists();
  updateDriver(true);
  updateEffects(true);
}

//when changing audio drivers, device/frequency/latency values may no longer be valid for new driver
//updateDriverLists() will try to select a match if one is found
//otherwise, this function will force now-invalid settings to the first setting in each list
auto AudioSettings::updateDriver(bool initializing) -> void {
  settings["Audio/Device"].setValue(deviceList.selected().text());
  settings["Audio/Frequency"].setValue(frequencyList.selected().text());
  settings["Audio/Latency"].setValue(latencyList.selected().text());
  settings["Audio/Exclusive"].setValue(exclusiveMode.checked());

  if(!initializing) program->updateAudioDriver();
}

auto AudioSettings::updateEffects(bool initializing) -> void {
  settings["Audio/Volume"].setValue(volumeSlider.position());
  volumeValue.setText({volumeSlider.position(), "%"});

  settings["Audio/Balance"].setValue(balanceSlider.position());
  balanceValue.setText({balanceSlider.position(), "%"});

  settings["Audio/Reverb/Enable"].setValue(reverbEnable.checked());

  if(!initializing) program->updateAudioEffects();
}

//called during initialization, and after changing audio device
//each audio device may have separately supported frequencies and/or latencies
auto AudioSettings::updateDriverLists() -> void {
  auto information = audio->information();

  deviceList.reset();
  for(auto& device : information.devices) {
    deviceList.append(ComboButtonItem().setText(device));
    if(device == settings["Audio/Device"].text()) {
      deviceList.item(deviceList.itemCount() - 1).setSelected();
    }
  }

  frequencyList.reset();
  for(auto& frequency : information.frequencies) {
    frequencyList.append(ComboButtonItem().setText(frequency));
    if(frequency == settings["Audio/Frequency"].real()) {
      frequencyList.item(frequencyList.itemCount() - 1).setSelected();
    }
  }

  latencyList.reset();
  for(auto& latency : information.latencies) {
    latencyList.append(ComboButtonItem().setText(latency));
    if(latency == settings["Audio/Latency"].natural()) {
      latencyList.item(latencyList.itemCount() - 1).setSelected();
    }
  }
}
