AudioSettings audioSettings;

void AudioSettings::create() {
  setTitle("Audio Settings");
  application.addWindow(this, "AudioSettings", "160,160");

  volumeLabel.setText("Volume:");
  volumeSlider.setLength(201);
  frequencyLabel.setText("Frequency:");
  frequencySlider.setLength(2001);

  layout.setMargin(5);
  volumeLayout.append(volumeLabel,        70, 0);
  volumeLayout.append(volumeValue,        60, 0);
  volumeLayout.append(volumeSlider,       ~0, 0);
  layout.append(volumeLayout                   );
  frequencyLayout.append(frequencyLabel,  70, 0);
  frequencyLayout.append(frequencyValue,  60, 0);
  frequencyLayout.append(frequencySlider, ~0, 0);
  layout.append(frequencyLayout);

  setGeometry({ 0, 0, 480, layout.minimumGeometry().height });
  append(layout);

  volumeSlider.onChange = []() {
    config.audio.volume = audioSettings.volumeSlider.position();
    audio.set(Audio::Volume, config.audio.volume);
    audioSettings.volumeValue.setText({ config.audio.volume, "%" });
  };

  frequencySlider.onChange = []() {
    config.audio.inputFrequency = audioSettings.frequencySlider.position() + 31000;
    audio.set(Audio::ResampleRatio, (double)config.audio.inputFrequency / (double)config.audio.outputFrequency);
    audioSettings.frequencyValue.setText({ config.audio.inputFrequency, "hz" });
  };

  volumeSlider.setPosition(config.audio.volume);
  volumeValue.setText({ config.audio.volume, "%" });

  frequencySlider.setPosition(config.audio.inputFrequency - 31000);
  frequencyValue.setText({ config.audio.inputFrequency, "hz" });
}
