auto VideoSettings::create() -> void {
  setIcon(Icon::Device::Display);
  setText("Video");

  layout.setPadding(5);

  colorAdjustmentLabel.setFont(Font().setBold()).setText("Color Adjustment");
  colorLayout.setSize({3, 3});
  colorLayout.column(0).setAlignment(1.0);
  luminanceLabel.setText("Luminance:");
  luminanceValue.setAlignment(0.5);
  luminanceSlider.setLength(101).setPosition(settings["Video/Luminance"].natural()).onChange([&] {
    string value = {luminanceSlider.position(), "%"};
    settings["Video/Luminance"].setValue(value);
    luminanceValue.setText(value);
    program.updateVideoPalette();
  }).doChange();
  saturationLabel.setText("Saturation:");
  saturationValue.setAlignment(0.5);
  saturationSlider.setLength(201).setPosition(settings["Video/Saturation"].natural()).onChange([&] {
    string value = {saturationSlider.position(), "%"};
    settings["Video/Saturation"].setValue(value);
    saturationValue.setText(value);
    program.updateVideoPalette();
  }).doChange();
  gammaLabel.setText("Gamma:");
  gammaValue.setAlignment(0.5);
  gammaSlider.setLength(101).setPosition(settings["Video/Gamma"].natural() - 100).onChange([&] {
    string value = {100 + gammaSlider.position(), "%"};
    settings["Video/Gamma"].setValue(value);
    gammaValue.setText(value);
    program.updateVideoPalette();
  }).doChange();
}
