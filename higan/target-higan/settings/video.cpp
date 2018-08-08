VideoSettings::VideoSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Display);
  setText("Video");

  layout.setPadding(5);

  colorAdjustmentLabel.setFont(Font().setBold()).setText("Color Adjustment");
  saturationLabel.setText("Saturation:");
  saturationValue.setAlignment(0.5);
  saturationSlider.setLength(201).setPosition(settings["Video/Saturation"].natural()).onChange([&] { updateColor(); });
  gammaLabel.setText("Gamma:");
  gammaValue.setAlignment(0.5);
  gammaSlider.setLength(101).setPosition(settings["Video/Gamma"].natural() - 100).onChange([&] { updateColor(); });
  luminanceLabel.setText("Luminance:");
  luminanceValue.setAlignment(0.5);
  luminanceSlider.setLength(101).setPosition(settings["Video/Luminance"].natural()).onChange([&] { updateColor(); });

  overscanMaskLabel.setFont(Font().setBold()).setText("Overscan Area");
  horizontalMaskLabel.setText("Horizontal:");
  horizontalMaskValue.setAlignment(0.5);
  horizontalMaskSlider.setLength(25).setPosition(settings["View/Overscan/Horizontal"].natural()).onChange([&] { updateOverscan(); });
  verticalMaskLabel.setText("Vertical:");
  verticalMaskValue.setAlignment(0.5);
  verticalMaskSlider.setLength(25).setPosition(settings["View/Overscan/Vertical"].natural()).onChange([&] { updateOverscan(); });

  fullscreenLabel.setFont(Font().setBold()).setText("Fullscreen");
  fullscreenExclusive.setText("Exclusive mode").setChecked(settings["Video/Exclusive"].boolean()).onToggle([&] {
    settings["Video/Exclusive"].setValue(fullscreenExclusive.checked());
  });

  updateColor(true);
  updateOverscan(true);
}

auto VideoSettings::updateColor(bool initializing) -> void {
  settings["Video/Saturation"].setValue(saturationSlider.position());
  settings["Video/Gamma"].setValue(100 + gammaSlider.position());
  settings["Video/Luminance"].setValue(luminanceSlider.position());
  saturationValue.setText({saturationSlider.position(), "%"});
  gammaValue.setText({100 + gammaSlider.position(), "%"});
  luminanceValue.setText({luminanceSlider.position(), "%"});

  if(!initializing) program->updateVideoPalette();
}

auto VideoSettings::updateOverscan(bool initializing) -> void {
  settings["View/Overscan/Horizontal"].setValue(horizontalMaskSlider.position());
  settings["View/Overscan/Vertical"].setValue(verticalMaskSlider.position());
  horizontalMaskValue.setText({horizontalMaskSlider.position()});
  verticalMaskValue.setText({verticalMaskSlider.position()});

  if(initializing || settings["View/Overscan"].boolean()) return;
  if(settings["View/Adaptive"].boolean()) {
    presentation->resizeWindow();
  } else {
    presentation->resizeViewport();
  }
}
