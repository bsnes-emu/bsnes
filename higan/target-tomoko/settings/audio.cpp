AudioSettings::AudioSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Speaker);
  setText("Audio");

  layout.setMargin(5);

  driverLabel.setFont(Font().setBold()).setText("Driver");

  deviceLabel.setText("Device:");
  deviceList.onChange([&] { updateDevice(); updateDriver(); });

  //the device list never changes once a driver is activated;
  //however, the available frequencies and latencies may change when the active device is changed
  for(auto& device : audio->information().devices) {
    deviceList.append(ComboButtonItem().setText(device));
    if(device == settings["Audio/Device"].text()) {
      deviceList.item(deviceList.itemCount() - 1).setSelected();
    }
  }

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

  updateDevice();
  updateDriver(true);
  updateEffects(true);
}

auto AudioSettings::updateDevice() -> void {
  frequencyList.reset();
  for(auto& frequency : audio->information().frequencies) {
    frequencyList.append(ComboButtonItem().setText(frequency));
    if(frequency == settings["Audio/Frequency"].real()) {
      frequencyList.item(frequencyList.itemCount() - 1).setSelected();
    }
  }

  latencyList.reset();
  for(auto& latency : audio->information().latencies) {
    latencyList.append(ComboButtonItem().setText(latency));
    if(latency == settings["Audio/Latency"].natural()) {
      latencyList.item(latencyList.itemCount() - 1).setSelected();
    }
  }
}

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
