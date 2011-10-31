VideoSettings *videoSettings = 0;

VideoSlider::VideoSlider() {
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
  brightness.slider.setLength(201);
  contrast.name.setText("Contrast:");
  contrast.slider.setLength(201);
  gamma.name.setText("Gamma:");
  gamma.slider.setLength(101);
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
  compositorLabel.setText("Disable window compositor:");
  compositorLabel.setFont(application->boldFont);
  compositor[0].setText("Never");
  compositor[1].setText("Fullscreen");
  compositor[2].setText("Always");
  RadioBox::group(compositor[0], compositor[1], compositor[2]);

  append(title, ~0, 0, 5);
  append(colorAdjustment, ~0, 0);
  append(brightness, ~0, 0);
  append(contrast, ~0, 0);
  append(gamma, ~0, 0, 5);
  append(overscanAdjustment, ~0, 0);
  append(overscanHorizontal, ~0, 0);
  append(overscanVertical, ~0, 0, 5);
  append(fullScreenMode, ~0, 0);
  append(fullScreenLayout, ~0, 0, 5);
    fullScreenLayout.append(fullScreen[0], ~0, 0, 5);
    fullScreenLayout.append(fullScreen[1], ~0, 0, 5);
    fullScreenLayout.append(fullScreen[2], ~0, 0);
  append(compositorLabel, ~0, 0);
  append(compositorLayout, ~0, 0);
    compositorLayout.append(compositor[0], ~0, 0, 5);
    compositorLayout.append(compositor[1], ~0, 0, 5);
    compositorLayout.append(compositor[2], ~0, 0);

  brightness.slider.setPosition(config->video.brightness);
  contrast.slider.setPosition(config->video.contrast);
  gamma.slider.setPosition(config->video.gamma);
  overscanHorizontal.slider.setPosition(config->video.maskOverscanHorizontal);
  overscanVertical.slider.setPosition(config->video.maskOverscanVertical);
  fullScreen[config->video.fullScreenMode].setChecked();
  compositor[config->video.compositionMode].setChecked();

  synchronize();

  brightness.slider.onChange = contrast.slider.onChange = gamma.slider.onChange =
  overscanHorizontal.slider.onChange = overscanVertical.slider.onChange =
  { &VideoSettings::synchronize, this };

  fullScreen[0].onTick = [&] { config->video.fullScreenMode = 0; };
  fullScreen[1].onTick = [&] { config->video.fullScreenMode = 1; };
  fullScreen[2].onTick = [&] { config->video.fullScreenMode = 2; };

  compositor[0].onTick = [&] {
    config->video.compositionMode = 0;
    compositor::enable(application->compositionEnable);
  };

  compositor[1].onTick = [&] {
    config->video.compositionMode = 1;
    compositor::enable(application->compositionEnable && mainWindow->fullScreen() == false);
  };

  compositor[2].onTick = [&] {
    config->video.compositionMode = 2;
    compositor::enable(false);
  };
}

void VideoSettings::synchronize() {
  config->video.brightness = brightness.slider.position();
  config->video.contrast = contrast.slider.position();
  config->video.gamma = gamma.slider.position();
  config->video.maskOverscanHorizontal = overscanHorizontal.slider.position();
  config->video.maskOverscanVertical = overscanVertical.slider.position();
  if(fullScreen[0].checked()) { config->video.fullScreenMode = 0;  }
  if(fullScreen[1].checked()) { config->video.fullScreenMode = 1; }
  if(fullScreen[2].checked()) { config->video.fullScreenMode = 2;  }

  brightness.value.setText({ config->video.brightness, "%" });
  contrast.value.setText({ config->video.contrast, "%" });
  gamma.value.setText({ 100 + config->video.gamma, "%" });

  overscanHorizontal.value.setText({ config->video.maskOverscanHorizontal, "px" });
  overscanVertical.value.setText({ config->video.maskOverscanVertical, "px" });

  palette.update();
}
