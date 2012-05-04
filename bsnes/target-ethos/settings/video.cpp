VideoSettings *videoSettings = nullptr;

VideoSlider::VideoSlider() {
  append(name, {75, 0});
  append(value, {75, 0});
  append(slider, {~0, 0});
}

VideoSettings::VideoSettings() {
  title.setFont(application->titleFont);
  title.setText("Video Settings");
  colorAdjustment.setFont(application->boldFont);
  colorAdjustment.setText("Color adjustment:");
  saturation.name.setText("Saturation:");
  saturation.slider.setLength(201);
  gamma.name.setText("Gamma:");
  gamma.slider.setLength(101);
  luminance.name.setText("Luminance:");
  luminance.slider.setLength(101);
  overscanAdjustment.setFont(application->boldFont);
  overscanAdjustment.setText("Overscan mask:");
  overscanHorizontal.name.setText("Horizontal:");
  overscanHorizontal.slider.setLength(17);
  overscanVertical.name.setText("Vertical:");
  overscanVertical.slider.setLength(17);

  append(title, {~0, 0}, 5);
  append(colorAdjustment, {~0, 0});
  append(saturation, {~0, 0});
  append(gamma, {~0, 0});
  append(luminance, {~0, 0}, 5);
  append(overscanAdjustment, {~0, 0});
  append(overscanHorizontal, {~0, 0});
  append(overscanVertical, {~0, 0}, 5);

  saturation.slider.setPosition(config->video.saturation);
  gamma.slider.setPosition(config->video.gamma - 100);
  luminance.slider.setPosition(config->video.luminance);
  overscanHorizontal.slider.setPosition(config->video.maskOverscanHorizontal);
  overscanVertical.slider.setPosition(config->video.maskOverscanVertical);

  synchronize();

  saturation.slider.onChange = gamma.slider.onChange = luminance.slider.onChange =
  overscanHorizontal.slider.onChange = overscanVertical.slider.onChange =
  {&VideoSettings::synchronize, this};
}

void VideoSettings::synchronize() {
  config->video.saturation = saturation.slider.position();
  config->video.gamma = 100 + gamma.slider.position();
  config->video.luminance = luminance.slider.position();
  config->video.maskOverscanHorizontal = overscanHorizontal.slider.position();
  config->video.maskOverscanVertical = overscanVertical.slider.position();

  saturation.value.setText({config->video.saturation, "%"});
  gamma.value.setText({config->video.gamma, "%"});
  luminance.value.setText({config->video.luminance, "%"});
  overscanHorizontal.value.setText({config->video.maskOverscanHorizontal, "px"});
  overscanVertical.value.setText({config->video.maskOverscanVertical, "px"});

  if(application->active) system().updatePalette();
}
