bool SettingsWindow::close() {
  hide();
  return false;
}

void SettingsWindow::setup() {
  create("center", 640, 365, "bsnes Configuration Settings");

  attach(panel.create("", 150, 355), 5, 5);
  panel.add_item("Video Settings");
  panel.add_item("Color Adjustment");
  panel.add_item("Raster Settings");
  panel.add_item("Emulation Settings");
  panel.add_item("Input Configuration");
  panel.add_item("Cheat Code Editor");
  panel.autosize_columns();
}
