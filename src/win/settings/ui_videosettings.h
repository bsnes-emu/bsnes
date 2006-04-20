class VideoSettingsWindow : public Window {
public:
Label    Header;
Label    VideoProfileLabel;
Combobox VideoProfile;
Label    WinProfileLabel;
Combobox WinProfile;
Label    FullProfileLabel;
Combobox FullProfile;
Label    Separator1;
Label    SoftwareFilterLabel;
Combobox SoftwareFilter;
Label    HardwareFilterLabel;
Combobox HardwareFilter;
Label    VideoStandardLabel;
Combobox VideoStandard;
Label    MultiplierLabel;
Combobox Multiplier;
Checkbox FixAspectRatio;
Checkbox Scanlines;
Checkbox ManualRenderSize;
Label    RenderWidthLabel;
Editbox  RenderWidth;
Label    RenderHeightLabel;
Editbox  RenderHeight;
Label    Separator2;
Checkbox Fullscreen;
Label    FullResWidthLabel;
Editbox  FullResWidth;
Label    FullResHeightLabel;
Editbox  FullResHeight;
Label    FullResHzLabel;
Editbox  FullResHz;
Checkbox TripleBuffering;
Button   ApplySettings;
Button   SelectProfile;

  bool Event(EventInfo &info);

  void LoadSettings(uint profile);
  void SaveSettings(uint profile);
  void Show();
  void Setup();
} wVideoSettings;
