bool VideoSettingsWindow::message(uint id, uintptr_t param) {
  return true;
}

void VideoSettingsWindow::setup() {
  create(0, 475, 355);

int x = 0, y = 0;
int lh = ui::Label::ideal_height;
int ch = ui::Combobox::ideal_height;
int kh = ui::Checkbox::ideal_height;
int eh = ui::Editbox::ideal_height;
  label_select.create(*this, 0, x, y, 475, lh, "Select video mode to configure:");
  y += lh;
  mode_select.create(*this, 0, x, y, 475, ch);
  mode_select.add_item("Windowed mode");
  mode_select.add_item("Pseudo-fullscreen mode");
  mode_select.add_item("Fullscreen mode");
  y += ch + 5;

  label_region.create(*this, 0, x, y, 235, lh, "Region:");
  label_scalar.create(*this, 0, x + 240, y, 235, lh, "Scale:");
  y += lh;
  region.create(*this, 0, x, y, 235, ch);
  region.add_item("NTSC");
  region.add_item("PAL");
  scalar.create(*this, 0, x + 240, y, 235, ch);
  scalar.add_item("100%");
  scalar.add_item("200%");
  scalar.add_item("300%");
  scalar.add_item("400%");
  scalar.add_item("500%");
  y += ch;

  label_hwfilter.create(*this, 0, x, y, 235, lh, "Hardware filter:");
  label_swfilter.create(*this, 0, x + 240, y, 235, lh, "Software filter:");
  y += lh;
  hwfilter.create(*this, 0, x, y, 235, ch);
  hwfilter.add_item("Point");
  hwfilter.add_item("Linear");
  swfilter.create(*this, 0, x + 240, y, 235, ch);
  swfilter.add_item("None");
  swfilter.add_item("NTSC");
  swfilter.add_item("HQ2x");
  swfilter.add_item("Scale2x");
  y += ch + 5;

  aspect.create(*this, 0, x, y, 235, kh, "Correct aspect ratio");
  vsync.create(*this, 0, x + 240, y, 235, kh, "Sync to vertical retrace");
  y += kh + 5;

  label_fsmode.create(*this, 0, x, y, 475, lh, "Fullscreen mode (width x height x refresh rate):");
  y += lh;

  fs_width.create(*this, 0, x, y, 155, eh, "<width>");
  fs_height.create(*this, 0, x + 160, y, 155, eh, "<height>");
  fs_refresh.create(*this, 0, x + 320, y, 155, eh, "<refresh_rate>");
  y += eh + 5;
}
