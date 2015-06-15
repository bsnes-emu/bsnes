TimingSettings::TimingSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Clock);
  setText("Timing");

  layout.setMargin(5);
  videoLabel.setText("Video:");
  videoValue.setText(config->timing.video).onActivate([&] { update(); });
  videoAssign.setText("Assign").onActivate([&] { update(); });
  audioLabel.setText("Audio:");
  audioValue.setText(config->timing.audio).onActivate([&] { update(); });
  audioAssign.setText("Assign").onActivate([&] { update(); });
}

auto TimingSettings::update() -> void {
  config->timing.video = real(videoValue.text());
  config->timing.audio = real(audioValue.text());
  program->updateDSP();
}
