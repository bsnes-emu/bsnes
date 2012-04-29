AudioSettings *audioSettings = nullptr;

AudioSettings::AudioSettings() {
  title.setFont(application->titleFont);
  title.setText("Audio Settings");

  append(title, {~0, 0}, 5);
}
