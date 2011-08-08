AudioSettings audioSettings;

void AudioSettings::create() {
  title.setText("Audio Settings");
  title.setFont(application.titleFont);

  volumeLabel.setText("Volume:");
  volumeSlider.setLength(201);
  frequencyLabel.setText("Frequency:");
  frequencySlider.setLength(2001);

  panelLayout.setMargin(5);
  panelLayout.append(panel, SettingsWindow::PanelWidth, ~0, 5);
  panelLayout.append(layout);

  layout.append(title, ~0, 0, 5);

  volumeLayout.append(volumeLabel, 70, 0);
  volumeLayout.append(volumeValue, 60, 0);
  volumeLayout.append(volumeSlider, ~0, 0);
  layout.append(volumeLayout);

  frequencyLayout.append(frequencyLabel, 70, 0);
  frequencyLayout.append(frequencyValue, 60, 0);
  frequencyLayout.append(frequencySlider, ~0, 0);
  layout.append(frequencyLayout);

  layout.append(spacer, ~0, ~0);

  volumeSlider.onChange = [this]() {
    config.audio.volume = volumeSlider.position();
    audio.set(Audio::Volume, config.audio.volume);
    volumeValue.setText({ config.audio.volume, "%" });
  };

  frequencySlider.onChange = [this]() {
    config.audio.inputFrequency = frequencySlider.position() + 31000;
    audio.set(Audio::ResampleRatio, (double)config.audio.inputFrequency / (double)config.audio.outputFrequency);
    frequencyValue.setText({ config.audio.inputFrequency, "hz" });
  };

  volumeSlider.setPosition(config.audio.volume);
  volumeValue.setText({ config.audio.volume, "%" });

  frequencySlider.setPosition(config.audio.inputFrequency - 31000);
  frequencyValue.setText({ config.audio.inputFrequency, "hz" });
}
