bool RasterSettingsWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_CHANGED: {
    if(info.control == &PScanlineSlider) {
    } else if(info.control == &IScanlineSlider) {
    }
  } break;

  }

  return false;
}

void RasterSettingsWindow::Setup() {
  Header.Create(this, "visible", 0, 0, 475, 30, "Raster Settings");
  Header.SetFont(global::font_header);

int x = 15, y = 30;
  ScanlineLabel.Create(this, "visible", x, y, 460, 15,
    "Scanline intensity adjust: (0% = no scanlines, 100% = full scanlines)");
  y += 20;

  PScanlineLabel.Create(this, "visible", x, y, 100, 15);
  PScanlineSlider.Create(this, "visible", x + 105, y - 3, 355, 25);
  PScanlineSlider.SetRange(0, 100);
  y += 25;

  IScanlineLabel.Create(this, "visible", x, y, 100, 15);
  IScanlineSlider.Create(this, "visible", x + 105, y - 3, 355, 25);
  IScanlineSlider.SetRange(0, 100);
  y += 25;
}
