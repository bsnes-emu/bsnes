VideoSettings::VideoSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Display);
  setText("Video");

  layout.setMargin(5);

  colorAdjustmentLabel.setFont(Font().setBold()).setText("Color Adjustment");
  saturationLabel.setText("Saturation:");
  saturationValue.setAlignment(0.5);
  saturationSlider.setLength(201).setPosition(settings["Video/Saturation"].natural()).onChange([&] { updateColor(); });
  gammaLabel.setText("Gamma:");
  gammaValue.setAlignment(0.5);
  gammaSlider.setLength(101).setPosition(settings["Video/Gamma"].natural() - 100).onChange([&] { updateColor(); });
  luminanceLabel.setText("Luminance:");
  luminanceValue.setAlignment(0.5);
  luminanceSlider.setLength(101).setPosition(settings["Video/Luminance"].natural()).onChange([&] { updateColor(); });

  overscanMaskLabel.setFont(Font().setBold()).setText("Overscan Mask");
  horizontalMaskLabel.setText("Horizontal:");
  horizontalMaskValue.setAlignment(0.5);
  horizontalMaskSlider.setLength(25).setPosition(settings["Video/Overscan/Horizontal"].natural()).onChange([&] { updateViewport(); });
  verticalMaskLabel.setText("Vertical:");
  verticalMaskValue.setAlignment(0.5);
  verticalMaskSlider.setLength(25).setPosition(settings["Video/Overscan/Vertical"].natural()).onChange([&] { updateViewport(); });

  windowedModeLabel.setFont(Font().setBold()).setText("Windowed Mode");
  windowedModeAspectCorrection.setText("Aspect correction").setChecked(settings["Video/Windowed/AspectCorrection"].boolean()).onToggle([&] { updateViewport(); });
  windowedModeIntegralScaling.setText("Integral scaling").setChecked(settings["Video/Windowed/IntegralScaling"].boolean()).onToggle([&] { updateViewport(); });
  windowedModeAdaptive.setText("Adaptive sizing").setChecked(settings["Video/Windowed/Adaptive"].boolean()).onToggle([&] { updateViewport(); });

  fullscreenModeLabel.setFont(Font().setBold()).setText("Fullscreen Mode");
  fullscreenModeAspectCorrection.setText("Aspect correction").setChecked(settings["Video/Fullscreen/AspectCorrection"].boolean()).onToggle([&] { updateViewport(); });
  fullscreenModeIntegralScaling.setText("Integral scaling").setChecked(settings["Video/Fullscreen/IntegralScaling"].boolean()).onToggle([&] { updateViewport(); });
  fullscreenModeExclusive.setText("Exclusive mode").setChecked(settings["Video/Fullscreen/Exclusive"].boolean()).onToggle([&] { updateViewport(); });
  if(!video->cap(Video::Exclusive)) fullscreenModeExclusive.remove();

  updateColor(true);
  updateViewport(true);
}

auto VideoSettings::updateColor(bool initializing) -> void {
  settings["Video/Saturation"].setValue(saturationSlider.position());
  settings["Video/Gamma"].setValue(100 + gammaSlider.position());
  settings["Video/Luminance"].setValue(luminanceSlider.position());
  saturationValue.setText({saturationSlider.position(), "%"});
  gammaValue.setText({100 + gammaSlider.position(), "%"});
  luminanceValue.setText({luminanceSlider.position(), "%"});

  if(!initializing) program->updateVideoPalette();
}

auto VideoSettings::updateViewport(bool initializing) -> void {
  bool wasAdaptive = settings["Video/Windowed/Adaptive"].boolean();
  bool isAdaptive = windowedModeAdaptive.checked();

  settings["Video/Overscan/Horizontal"].setValue(horizontalMaskSlider.position());
  settings["Video/Overscan/Vertical"].setValue(verticalMaskSlider.position());
  settings["Video/Windowed/AspectCorrection"].setValue(windowedModeAspectCorrection.checked());
  settings["Video/Windowed/IntegralScaling"].setValue(windowedModeIntegralScaling.checked());
  settings["Video/Windowed/Adaptive"].setValue(windowedModeAdaptive.checked());
  settings["Video/Fullscreen/AspectCorrection"].setValue(fullscreenModeAspectCorrection.checked());
  settings["Video/Fullscreen/IntegralScaling"].setValue(fullscreenModeIntegralScaling.checked());
  settings["Video/Fullscreen/Exclusive"].setValue(fullscreenModeExclusive.checked());
  horizontalMaskValue.setText({horizontalMaskSlider.position()});
  verticalMaskValue.setText({verticalMaskSlider.position()});

  if(!initializing) presentation->resizeViewport(isAdaptive || wasAdaptive != isAdaptive);
}
