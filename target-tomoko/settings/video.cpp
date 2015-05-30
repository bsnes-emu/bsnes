VideoSettings::VideoSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Display);
  setText("Video");

  layout.setMargin(5);
}
