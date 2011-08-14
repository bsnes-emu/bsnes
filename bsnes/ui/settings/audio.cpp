AudioSettings audioSettings;

void AudioSettings::create() {
  title.setText("Audio Settings");
  title.setFont(application.titleFont);

  frequencyLabel.setText("Frequency:");
  frequencySlider.setLength(2001);
  volumeLabel.setText("Volume:");
  volumeSlider.setLength(201);
  balanceLabel.setText("Balance:");
  balanceSlider.setLength(201);

  panelLayout.setMargin(5);
  panelLayout.append(panel, SettingsWindow::PanelWidth, ~0, 5);
  panelLayout.append(layout);

  layout.append(title, ~0, 0, 5);

  frequencyLayout.append(frequencyLabel, 70, 0);
  frequencyLayout.append(frequencyValue, 60, 0);
  frequencyLayout.append(frequencySlider, ~0, 0);
  layout.append(frequencyLayout);

  volumeLayout.append(volumeLabel, 70, 0);
  volumeLayout.append(volumeValue, 60, 0);
  volumeLayout.append(volumeSlider, ~0, 0);
  layout.append(volumeLayout);

  balanceLayout.append(balanceLabel, 70, 0);
  balanceLayout.append(balanceValue, 60, 0);
  balanceLayout.append(balanceSlider, ~0, 0);
  layout.append(balanceLayout);

  layout.append(spacer, ~0, ~0);
  settingsWindow.append(panelLayout);

  frequencySlider.onChange = [this] {
    config.audio.inputFrequency = frequencySlider.position() + 31000;
    dspaudio.set_frequency(config.audio.inputFrequency);
    frequencyValue.setText({ config.audio.inputFrequency, "hz" });
  };

  volumeSlider.onChange = [this] {
    config.audio.volume = volumeSlider.position();
    dspaudio.set_volume((double)config.audio.volume / 100.0);
    volumeValue.setText({ config.audio.volume, "%" });
  };

  balanceSlider.onChange = [this] {
    config.audio.balance = balanceSlider.position();
    dspaudio.set_balance((double)((signed)config.audio.balance - 100) / 100.0);
    balanceValue.setText({ (signed)config.audio.balance - 100 });
  };

  frequencySlider.setPosition(config.audio.inputFrequency - 31000);
  frequencyValue.setText({ config.audio.inputFrequency, "hz" });

  volumeSlider.setPosition(config.audio.volume);
  volumeValue.setText({ config.audio.volume, "%" });

  balanceSlider.setPosition(config.audio.balance);
  balanceValue.setText({ (signed)config.audio.balance - 100 });
}
