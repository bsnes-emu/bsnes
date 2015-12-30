VideoSettings::VideoSettings(TabFrame* parent) : TabFrameItem(parent) {
  setImage(Icon::Device::Display);
  setText("Video");

  layout.setMargin(5);

  colorAdjustmentLabel.setFont(Font().setBold()).setText("Color Adjustment");
  saturationLabel.setText("Saturation:");
  saturationSlider.setLength(201).setPosition(settings["Video/Saturation"].natural()).onChange([&] { update(); });
  gammaLabel.setText("Gamma:");
  gammaSlider.setLength(101).setPosition(settings["Video/Gamma"].natural() - 100).onChange([&] { update(); });
  luminanceLabel.setText("Luminance:");
  luminanceSlider.setLength(101).setPosition(settings["Video/Luminance"].natural()).onChange([&] { update(); });

  overscanMaskLabel.setFont(Font().setBold()).setText("Overscan Mask");
  horizontalMaskLabel.setText("Horizontal:");
  horizontalMaskSlider.setLength(17).setPosition(settings["Video/Overscan/Horizontal"].natural()).onChange([&] { update(); });
  verticalMaskLabel.setText("Vertical:");
  verticalMaskSlider.setLength(17).setPosition(settings["Video/Overscan/Vertical"].natural()).onChange([&] { update(); });

  update();
}

auto VideoSettings::update() -> void {
  settings["Video/Saturation"].setValue(saturationSlider.position());
  settings["Video/Gamma"].setValue(100 + gammaSlider.position());
  settings["Video/Luminance"].setValue(luminanceSlider.position());
  settings["Video/Overscan/Horizontal"].setValue(horizontalMaskSlider.position());
  settings["Video/Overscan/Vertical"].setValue(verticalMaskSlider.position());
  saturationValue.setText({saturationSlider.position(), "%"});
  gammaValue.setText({100 + gammaSlider.position(), "%"});
  luminanceValue.setText({luminanceSlider.position(), "%"});
  horizontalMaskValue.setText({horizontalMaskSlider.position(), "px"});
  verticalMaskValue.setText({verticalMaskSlider.position(), "px"});
  program->updateVideoPalette();
}
