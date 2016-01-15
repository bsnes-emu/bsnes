VideoSettings::VideoSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Display);
  setText("Video");

  layout.setMargin(5);

  overscanMaskLabel.setFont(Font().setBold()).setText("Overscan Mask");
  horizontalMaskLabel.setText("Horizontal:");
  horizontalMaskSlider.setLength(17).setPosition(settings["Video/Overscan/Horizontal"].natural()).onChange([&] { update(); });
  verticalMaskLabel.setText("Vertical:");
  verticalMaskSlider.setLength(17).setPosition(settings["Video/Overscan/Vertical"].natural()).onChange([&] { update(); });

  update();
}

auto VideoSettings::update() -> void {
  settings["Video/Overscan/Horizontal"].setValue(horizontalMaskSlider.position());
  settings["Video/Overscan/Vertical"].setValue(verticalMaskSlider.position());
  horizontalMaskValue.setText({horizontalMaskSlider.position(), "px"});
  verticalMaskValue.setText({verticalMaskSlider.position(), "px"});
}
