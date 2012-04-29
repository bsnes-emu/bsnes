VideoSettings *videoSettings = nullptr;

VideoSettings::VideoSettings() {
  title.setFont(application->titleFont);
  title.setText("Video Settings");

  append(title, {~0, 0}, 5);
}
