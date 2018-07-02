VideoSettings::VideoSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Display);
  setText("Video");

  layout.setMargin(5);

  colorAdjustmentLabel.setFont(Font().setBold()).setText("Color Adjustment");
  luminanceLabel.setAlignment(1.0).setText("Luminance:");
  luminanceValue.setAlignment(0.5);
  luminanceSlider.setLength(101).setPosition(settings["Video/Luminance"].natural()).onChange([&] {
    string value = {luminanceSlider.position(), "%"};
    settings["Video/Luminance"].setValue(value);
    luminanceValue.setText(value);
    program->updateVideoPalette();
  }).doChange();
  saturationLabel.setAlignment(1.0).setText("Saturation:");
  saturationValue.setAlignment(0.5);
  saturationSlider.setLength(201).setPosition(settings["Video/Saturation"].natural()).onChange([&] {
    string value = {saturationSlider.position(), "%"};
    settings["Video/Saturation"].setValue(value);
    saturationValue.setText(value);
    program->updateVideoPalette();
  }).doChange();
  gammaLabel.setAlignment(1.0).setText("Gamma:");
  gammaValue.setAlignment(0.5);
  gammaSlider.setLength(101).setPosition(settings["Video/Gamma"].natural() - 100).onChange([&] {
    string value = {100 + gammaSlider.position(), "%"};
    settings["Video/Gamma"].setValue(value);
    gammaValue.setText(value);
    program->updateVideoPalette();
  }).doChange();

  fullscreenLabel.setFont(Font().setBold()).setText("Fullscreen");
  exclusiveMode.setText("Exclusive mode").setChecked(settings["Video/Exclusive"].boolean()).onToggle([&] {
    settings["Video/Exclusive"].setValue(exclusiveMode.checked());
  });
}
