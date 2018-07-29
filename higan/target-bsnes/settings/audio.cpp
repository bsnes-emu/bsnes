AudioSettings::AudioSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Speaker);
  setText("Audio");

  layout.setPadding(5);

  effectsLabel.setFont(Font().setBold()).setText("Effects");
  effectsLayout.setSize({3, 3});
  effectsLayout.column(0).setAlignment(1.0);
  skewLabel.setText("Skew:");
  skewValue.setAlignment(0.5);
  skewSlider.setLength(10001).setPosition(settings["Audio/Skew"].integer() + 5000).onChange([&] {
    string value = {skewSlider.position() > 5000 ? "+" : "", (int)skewSlider.position() - 5000};
    settings["Audio/Skew"].setValue(value);
    skewValue.setText(value);
    if(audio) program->updateAudioFrequency();
  }).doChange();
  volumeLabel.setText("Volume:");
  volumeValue.setAlignment(0.5);
  volumeSlider.setLength(201).setPosition(settings["Audio/Volume"].natural()).onChange([&] {
    string value = {volumeSlider.position(), "%"};
    settings["Audio/Volume"].setValue(value);
    volumeValue.setText(value);
    if(audio) program->updateAudioEffects();
  }).doChange();
  balanceLabel.setText("Balance:");
  balanceValue.setAlignment(0.5);
  balanceSlider.setLength(101).setPosition(settings["Audio/Balance"].natural()).onChange([&] {
    string value = {balanceSlider.position(), "%"};
    settings["Audio/Balance"].setValue(value);
    balanceValue.setText(value);
    if(audio) program->updateAudioEffects();
  }).doChange();
}
