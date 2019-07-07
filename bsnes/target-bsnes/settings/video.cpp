auto VideoSettings::create() -> void {
  setIcon(Icon::Device::Display);
  setText("Video");

  layout.setPadding(5_sx);

  colorAdjustmentLabel.setFont(Font().setBold()).setText("Color Adjustment");
  colorLayout.setSize({3, 3});
  colorLayout.column(0).setAlignment(1.0);
  luminanceLabel.setText("Luminance:");
  luminanceValue.setAlignment(0.5);
  luminanceSlider.setLength(101).setPosition(settings.video.luminance).onChange([&] {
    string value = {luminanceSlider.position(), "%"};
    settings.video.luminance = value.natural();
    luminanceValue.setText(value);
    program.updateVideoPalette();
  }).doChange();
  saturationLabel.setText("Saturation:");
  saturationValue.setAlignment(0.5);
  saturationSlider.setLength(201).setPosition(settings.video.saturation).onChange([&] {
    string value = {saturationSlider.position(), "%"};
    settings.video.saturation = value.natural();
    saturationValue.setText(value);
    program.updateVideoPalette();
  }).doChange();
  gammaLabel.setText("Gamma:");
  gammaValue.setAlignment(0.5);
  gammaSlider.setLength(101).setPosition(settings.video.gamma - 100).onChange([&] {
    string value = {100 + gammaSlider.position(), "%"};
    settings.video.gamma = value.natural();
    gammaValue.setText(value);
    program.updateVideoPalette();
  }).doChange();

  fastForwardFrameSkip.setText("Skip frames while fast forwarding").setChecked(settings.video.fastForwardFrameSkip).setToolTip({
    "When using the fast forward hotkey, this option will enable a frame skip of 9.\n"
    "Frame skipping while fast forwarding allows a higher maximum frame skipping frame rate."
  }).onToggle([&] {
    settings.video.fastForwardFrameSkip = fastForwardFrameSkip.checked();
  });

  snowOption.setText("Draw snow effect when idle").setChecked(settings.video.snow).onToggle([&] {
    settings.video.snow = snowOption.checked();
    presentation.updateProgramIcon();
  });
}
