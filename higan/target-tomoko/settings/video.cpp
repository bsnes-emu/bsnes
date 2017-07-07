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
  horizontalMaskSlider.setLength(25).setPosition(settings["Video/Overscan/Horizontal"].natural()).onChange([&] { updateOverscan(); });
  verticalMaskLabel.setText("Vertical:");
  verticalMaskValue.setAlignment(0.5);
  verticalMaskSlider.setLength(25).setPosition(settings["Video/Overscan/Vertical"].natural()).onChange([&] { updateOverscan(); });

  windowedModeLabel.setFont(Font().setBold()).setText("Windowed Mode");
  windowedModeAspectCorrection.setText("Aspect correction").setChecked(settings["Video/Windowed/AspectCorrection"].boolean()).onToggle([&] { updateViewport(); });
  windowedModeIntegralScaling.setText("Integral scaling").setChecked(settings["Video/Windowed/IntegralScaling"].boolean()).onToggle([&] { updateViewport(); });
  windowedModeAdaptiveSizing.setText("Adaptive sizing").setChecked(settings["Video/Windowed/AdaptiveSizing"].boolean()).onToggle([&] { updateViewport(); });

  fullscreenModeLabel.setFont(Font().setBold()).setText("Fullscreen Mode");
  fullscreenModeAspectCorrection.setText("Aspect correction").setChecked(settings["Video/Fullscreen/AspectCorrection"].boolean()).onToggle([&] { updateViewport(); });
  fullscreenModeIntegralScaling.setText("Integral scaling").setChecked(settings["Video/Fullscreen/IntegralScaling"].boolean()).onToggle([&] { updateViewport(); });

  updateColor();
  updateOverscan();
  updateViewport();
}

auto VideoSettings::updateColor() -> void {
  settings["Video/Saturation"].setValue(saturationSlider.position());
  settings["Video/Gamma"].setValue(100 + gammaSlider.position());
  settings["Video/Luminance"].setValue(luminanceSlider.position());
  saturationValue.setText({saturationSlider.position(), "%"});
  gammaValue.setText({100 + gammaSlider.position(), "%"});
  luminanceValue.setText({luminanceSlider.position(), "%"});
  program->updateVideoPalette();
}

auto VideoSettings::updateOverscan() -> void {
  settings["Video/Overscan/Horizontal"].setValue(horizontalMaskSlider.position());
  settings["Video/Overscan/Vertical"].setValue(verticalMaskSlider.position());
  horizontalMaskValue.setText({horizontalMaskSlider.position()});
  verticalMaskValue.setText({verticalMaskSlider.position()});
  presentation->resizeViewport();
}

auto VideoSettings::updateViewport() -> void {
  settings["Video/Windowed/AspectCorrection"].setValue(windowedModeAspectCorrection.checked());
  settings["Video/Windowed/IntegralScaling"].setValue(windowedModeIntegralScaling.checked());
  settings["Video/Windowed/AdaptiveSizing"].setValue(windowedModeAdaptiveSizing.checked());
  settings["Video/Fullscreen/AspectCorrection"].setValue(fullscreenModeAspectCorrection.checked());
  settings["Video/Fullscreen/IntegralScaling"].setValue(fullscreenModeIntegralScaling.checked());
  presentation->resizeViewport();
}
