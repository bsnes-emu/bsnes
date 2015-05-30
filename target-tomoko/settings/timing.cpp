TimingSettings::TimingSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Clock);
  setText("Timing");

  layout.setMargin(5);
}
