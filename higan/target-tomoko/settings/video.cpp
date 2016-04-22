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
  horizontalMaskSlider.setLength(17).setPosition(settings["Video/Overscan/Horizontal"].natural()).onChange([&] { updateOverscan(); });
  verticalMaskLabel.setText("Vertical:");
  verticalMaskValue.setAlignment(0.5);
  verticalMaskSlider.setLength(17).setPosition(settings["Video/Overscan/Vertical"].natural()).onChange([&] { updateOverscan(); });

  updateColor();
  updateOverscan();
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
  horizontalMaskValue.setText({horizontalMaskSlider.position(), "px"});
  verticalMaskValue.setText({verticalMaskSlider.position(), "px"});
}
