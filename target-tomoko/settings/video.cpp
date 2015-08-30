VideoSettings::VideoSettings(TabFrame* parent) : TabFrameItem(parent) {
  setImage(Icon::Device::Display);
  setText("Video");

  layout.setMargin(5);

  colorAdjustmentLabel.setFont(Font().setBold()).setText("Color Adjustment");
  saturationLabel.setText("Saturation:");
  saturationSlider.setLength(201).setPosition(config->video.saturation).onChange([&] { update(); });
  gammaLabel.setText("Gamma:");
  gammaSlider.setLength(101).setPosition(config->video.gamma - 100).onChange([&] { update(); });
  luminanceLabel.setText("Luminance:");
  luminanceSlider.setLength(101).setPosition(config->video.luminance).onChange([&] { update(); });

  overscanMaskLabel.setFont(Font().setBold()).setText("Overscan Mask");
  horizontalMaskLabel.setText("Horizontal:");
  horizontalMaskSlider.setLength(17).setPosition(config->video.overscan.horizontal).onChange([&] { update(); });
  verticalMaskLabel.setText("Vertical:");
  verticalMaskSlider.setLength(17).setPosition(config->video.overscan.vertical).onChange([&] { update(); });

  update();
}

auto VideoSettings::update() -> void {
  config->video.saturation = saturationSlider.position();
  config->video.gamma = 100 + gammaSlider.position();
  config->video.luminance = luminanceSlider.position();
  config->video.overscan.horizontal = horizontalMaskSlider.position();
  config->video.overscan.vertical = verticalMaskSlider.position();
  saturationValue.setText({config->video.saturation, "%"});
  gammaValue.setText({config->video.gamma, "%"});
  luminanceValue.setText({config->video.luminance, "%"});
  horizontalMaskValue.setText({config->video.overscan.horizontal, "px"});
  verticalMaskValue.setText({config->video.overscan.vertical, "px"});
  program->updateVideoPalette();
}
