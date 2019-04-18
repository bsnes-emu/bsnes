auto AudioSettings::create() -> void {
  setIcon(Icon::Device::Speaker);
  setText("Audio");

  layout.setPadding(5_sx);

  effectsLabel.setFont(Font().setBold()).setText("Effects");
  effectsLayout.setSize({3, 3});
  effectsLayout.column(0).setAlignment(1.0);
  skewLabel.setText("Skew:").setToolTip(
    "Adjusts the audio frequency by the skew amount (in Hz.)\n\n"
    "This is essentially static rate control:\n"
    "First, enable both video and audio sync.\n"
    "Then, raise or lower this value to try to reduce errors.\n"
    "One direction will help video, but hurt audio.\n"
    "The other direction will do the reverse.\n"
    "The idea is to find the best middle ground.\n\n"
    "You should leave this at 0 when using dynamic rate control."
  );
  skewValue.setAlignment(0.5).setToolTip(skewLabel.toolTip());
  skewSlider.setLength(10001).setPosition(settings.audio.skew + 5000).onChange([&] {
    string value = {skewSlider.position() > 5000 ? "+" : "", (int)skewSlider.position() - 5000};
    settings.audio.skew = value.integer();
    skewValue.setText(value);
    program.updateAudioFrequency();
  }).doChange();
  volumeLabel.setText("Volume:").setToolTip(
    "Adjusts the audio output volume.\n\n"
    "You should not use values above 100%, if possible!\n"
    "If you do, audio clipping distortion can occur."
  );
  volumeValue.setAlignment(0.5).setToolTip(volumeLabel.toolTip());
  volumeSlider.setLength(201).setPosition(settings.audio.volume).onChange([&] {
    string value = {volumeSlider.position(), "%"};
    settings.audio.volume = value.natural();
    volumeValue.setText(value);
    program.updateAudioEffects();
  }).doChange();
  balanceLabel.setText("Balance:").setToolTip(
    "Pans audio to the left (lower values) or right (higher values.)\n\n"
    "50% (centered) is the recommended setting."
  );
  balanceValue.setAlignment(0.5).setToolTip(balanceLabel.toolTip());
  balanceSlider.setLength(101).setPosition(settings.audio.balance).onChange([&] {
    string value = {balanceSlider.position(), "%"};
    settings.audio.balance = value.natural();
    balanceValue.setText(value);
    program.updateAudioEffects();
  }).doChange();
}
