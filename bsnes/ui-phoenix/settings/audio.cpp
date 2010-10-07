AudioSettings audioSettings;

void AudioSettings::create() {
  Window::create(0, 0, 256, 256, "Audio Settings");
  application.addWindow(this, "AudioSettings", "160,160");

  unsigned x = 5, y = 5;

  volumeLabel.create(*this, x, y, 70, Style::SliderHeight, "Volume:");
  volumeValue.create(*this, x + 70, y, 60, Style::SliderHeight, "100%");
  volumeSlider.create(*this, x + 130, y, 300, Style::SliderHeight, 201); y += Style::SliderHeight + 5;
  volumeSlider.setPosition(config.audio.volume);

  frequencyLabel.create(*this, x, y, 70, Style::SliderHeight, "Frequency:");
  frequencyValue.create(*this, x + 70, y, 60, Style::SliderHeight, "32000hz");
  frequencySlider.create(*this, x + 130, y, 300, Style::SliderHeight, 2001); y += Style::SliderHeight + 5;
  frequencySlider.setPosition(config.audio.inputFrequency - 31000);

  volumeSlider.onChange = []() {
    config.audio.volume = audioSettings.volumeSlider.position();
    audio.set(Audio::Volume, config.audio.volume);
    audioSettings.volumeValue.setText(string(config.audio.volume, "%"));
  };

  frequencySlider.onChange = []() {
    config.audio.inputFrequency = audioSettings.frequencySlider.position() + 31000;
    audio.set(Audio::ResampleRatio, (double)config.audio.inputFrequency / (double)config.audio.outputFrequency);
    audioSettings.frequencyValue.setText(string(config.audio.inputFrequency, "hz"));
  };

  setGeometry(0, 0, 440, y);
}
