TimingSettings::TimingSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Clock);
  setText("Timing");

  layout.setMargin(5);
  videoLabel.setText("Video:");
  videoValue.setText(settings["Timing/Video"].real()).onActivate([&] { update(); });
  videoAssign.setText("Assign").onActivate([&] { update(); });
  audioLabel.setText("Audio:");
  audioValue.setText(settings["Timing/Audio"].real()).onActivate([&] { update(); });
  audioAssign.setText("Assign").onActivate([&] { update(); });
}

auto TimingSettings::update() -> void {
  settings["Timing/Video"].setValue(videoValue.text().real());
  settings["Timing/Audio"].setValue(audioValue.text().real());
  program->updateDSP();
}
