bool VideoSettingsWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_CHANGED:
  case EVENT_CLICKED: {
    if(info.control == &VideoProfile) {
    //LoadSettings(VideoProfile.GetSelection());
    } else if(info.control == &ApplySettings) {
    //SaveSettings(VideoProfile.GetSelection());
    } else if(info.control == &SelectProfile) {
    //event::set_video_profile(VideoProfile.GetSelection());
    } else if(info.control == &ManualRenderSize) {
      UpdateControls();
    }
  } break;

  }

  return false;
}

void VideoSettingsWindow::UpdateControls() {
bool r = ManualRenderSize.Checked();
  Multiplier.Enable(!r);
  FixAspectRatio.Enable(!r);
  RenderWidth.Enable(r);
  RenderHeight.Enable(r);

  r = (VideoProfile.GetSelection() == 1);
  FullResWidth.Enable(r);
  FullResHeight.Enable(r);
  FullResHz.Enable(r);
}

void VideoSettingsWindow::Show() {
//VideoProfile.SetSelection(config::video.profile);
//LoadSettings(config::video.profile);
  Window::Show();
}

void VideoSettingsWindow::Setup() {
  Header.Create(this, "visible", 0, 0, 475, 30, "Video Settings");
  Header.SetFont(global::font_header);

int x = 15, y = 30;
  VideoProfileLabel.Create(this, "visible", x, y + 3, 135, 15, "Video profile to configure:");
  VideoProfile.Create(this, "visible", x + 135, y, 90, 200, "Windowed|Fullscreen");
  y += 26;

  Separator1.Create(this, "visible|sunken", x, y, 460, 3);
  y += 8;

  SoftwareFilterLabel.Create(this, "visible", x, y + 3, 85, 15, "Software filter:");
  SoftwareFilter.Create(this, "visible", x + 85, y, 140, 200,
    "None|NTSC|HQ2x|Scale2x");

  HardwareFilterLabel.Create(this, "visible", x + 235, y + 3, 85, 15, "Hardware filter:");
  HardwareFilter.Create(this, "visible", x + 320, y, 140, 200,
    "None|Bilinear");
  y += 25;

  VideoStandardLabel.Create(this, "visible", x, y + 3, 85, 15, "Video standard:");
  VideoStandard.Create(this, "visible", x + 85, y, 140, 200,
    "NTSC|PAL");

  MultiplierLabel.Create(this, "visible", x + 235, y + 3, 85, 15, "Multiplier:");
  Multiplier.Create(this, "visible", x + 320, y, 140, 200,
    "1x|2x|3x|4x|5x");
  y += 25;

  FixAspectRatio.Create(this, "visible|auto", x, y, 460, 16, "Correct aspect ratio");
  Scanlines.Create(this, "visible|auto", x, y + 15, 460, 15, "Enable hardware scanlines");
  ManualRenderSize.Create(this, "visible|auto", x, y + 30, 460, 15, "Manual render screen size");
  y += 47;

  RenderWidthLabel.Create(this, "visible", x, y + 3, 90, 15, "Render width:");
  RenderWidth.Create(this, "visible|edge", x + 90, y, 50, 20);
  RenderHeightLabel.Create(this, "visible", x + 150, y + 3, 90, 15, "Render height:");
  RenderHeight.Create(this, "visible|edge", x + 240, y, 50, 20);
  y += 25;

  Separator2.Create(this, "visible|sunken", x, y, 460, 3);
  y += 8;

  TripleBuffering.Create(this, "visible|auto", x, y, 460, 15, "Triple buffering (buggy, causes sound desync)");
  y += 20;

  FullResWidthLabel.Create(this, "visible", x, y + 3, 90, 15, "Resolution width:");
  FullResWidth.Create(this, "visible|edge", x + 90, y, 50, 20);
  FullResHeightLabel.Create(this, "visible", x + 150, y + 3, 90, 15, "Resolution height:");
  FullResHeight.Create(this, "visible|edge", x + 240, y, 50, 20);
  FullResHzLabel.Create(this, "visible", x + 300, y + 3, 70, 15, "Refresh rate:");
  FullResHz.Create(this, "visible|edge", x + 370, y, 50, 20);
  y += 27;

  ApplySettings.Create(this, "visible", x, y, 120, 25, "Apply settings");
  SelectProfile.Create(this, "visible", x + 125, y, 120, 25, "Set as active profile");
  y += 25;
}
