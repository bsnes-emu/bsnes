AudioSettings audioSettings;

void AudioSettings::create() {
  Window::create(0, 0, 480, 50, "Audio Settings");
  application.addWindow(this, "AudioSettings", "160,160");

  volumeLabel.setText("Volume:");
  volumeSlider.setLength(201);
  frequencyLabel.setText("Frequency:");
  frequencySlider.setLength(2001);

  layout.setMargin(5);
  volumeLayout.append(volumeLabel, 70, Style::SliderHeight);
  volumeLayout.append(volumeValue, 60, Style::SliderHeight);
  volumeLayout.append(volumeSlider, 0, Style::SliderHeight);
  layout.append(volumeLayout, 0, Style::SliderHeight);
  frequencyLayout.append(frequencyLabel, 70, Style::SliderHeight);
  frequencyLayout.append(frequencyValue, 60, Style::SliderHeight);
  frequencyLayout.append(frequencySlider, 0, Style::SliderHeight);
  layout.append(frequencyLayout, 0, Style::SliderHeight);
  setLayout(layout);

/*unsigned x = 5, y = 5;
  layout.append(volumeLabel,     x,       y,  70, Style::SliderHeight);
  layout.append(volumeValue,     x +  70, y,  60, Style::SliderHeight);
  layout.append(volumeSlider,    x + 130, y, 300, Style::SliderHeight); y += Style::SliderHeight + 5;
  layout.append(frequencyLabel,  x,       y,  70, Style::SliderHeight);
  layout.append(frequencyValue,  x +  70, y,  60, Style::SliderHeight);
  layout.append(frequencySlider, x + 130, y, 300, Style::SliderHeight); y += Style::SliderHeight + 5;
  setGeometry(0, 0, 440, y);
  setLayout(layout);*/

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
