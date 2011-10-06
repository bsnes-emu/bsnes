VideoSettings *videoSettings = 0;

VideoSlider::VideoSlider() {
  slider.setLength(201);

  append(name, 75, 0);
  append(value, 75, 0);
  append(slider, ~0, 0);
}

VideoSettings::VideoSettings() {
  title.setFont(application->titleFont);
  title.setText("Video Settings");
  colorAdjustment.setFont(application->boldFont);
  colorAdjustment.setText("Color adjustment:");
  brightness.name.setText("Brightness:");
  contrast.name.setText("Contrast:");
  gamma.name.setText("Gamma:");
  gammaRamp.setText("Enable gamma ramp simulation");
  overscanAdjustment.setFont(application->boldFont);
  overscanAdjustment.setText("Overscan mask:");
  overscanHorizontal.name.setText("Horizontal:");
  overscanHorizontal.slider.setLength(17);
  overscanVertical.name.setText("Vertical:");
  overscanVertical.slider.setLength(17);
  fullScreenMode.setFont(application->boldFont);
  fullScreenMode.setText("Fullscreen mode:");
  fullScreen[0].setText("Center");
  fullScreen[1].setText("Scale");
  fullScreen[2].setText("Stretch");
  RadioBox::group(fullScreen[0], fullScreen[1], fullScreen[2]);

  append(title, ~0, 0, 5);
  append(colorAdjustment, ~0, 0);
  append(brightness, ~0, 0);
  append(contrast, ~0, 0);
  append(gamma, ~0, 0);
  append(gammaRamp, ~0, 0, 5);
  append(overscanAdjustment, ~0, 0);
  append(overscanHorizontal, ~0, 0);
  append(overscanVertical, ~0, 0, 5);
  append(fullScreenMode, ~0, 0);
  append(fullScreenLayout, ~0, 0);
    fullScreenLayout.append(fullScreen[0], ~0, 0, 5);
    fullScreenLayout.append(fullScreen[1], ~0, 0, 5);
    fullScreenLayout.append(fullScreen[2], ~0, 0);

  brightness.slider.setPosition(config->video.brightness);
  contrast.slider.setPosition(config->video.contrast);
  gamma.slider.setPosition(config->video.gamma);
  gammaRamp.setChecked(config->video.gammaRamp);
  overscanHorizontal.slider.setPosition(config->video.maskOverscanHorizontal);
  overscanVertical.slider.setPosition(config->video.maskOverscanVertical);
  fullScreen[config->video.fullScreenMode].setChecked();

  synchronize();

  brightness.slider.onChange = contrast.slider.onChange = gamma.slider.onChange = gammaRamp.onTick =
  overscanHorizontal.slider.onChange = overscanVertical.slider.onChange =
  fullScreen[0].onTick = fullScreen[1].onTick = fullScreen[2].onTick =
  { &VideoSettings::synchronize, this };
}

void VideoSettings::synchronize() {
  config->video.brightness = brightness.slider.position();
  config->video.contrast = contrast.slider.position();
  config->video.gamma = gamma.slider.position();
  config->video.gammaRamp = gammaRamp.checked();
  config->video.maskOverscanHorizontal = overscanHorizontal.slider.position();
  config->video.maskOverscanVertical = overscanVertical.slider.position();
  if(fullScreen[0].checked()) config->video.fullScreenMode = 0;
  if(fullScreen[1].checked()) config->video.fullScreenMode = 1;
  if(fullScreen[2].checked()) config->video.fullScreenMode = 2;

  brightness.value.setText({ config->video.brightness, "%" });
  contrast.value.setText({ config->video.contrast, "%" });
  gamma.value.setText({ config->video.gamma, "%" });

  overscanHorizontal.value.setText({ config->video.maskOverscanHorizontal, "px" });
  overscanVertical.value.setText({ config->video.maskOverscanVertical, "px" });

  palette.update();
}
