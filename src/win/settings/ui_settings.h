enum {
  PANEL_VIDEOSETTINGS = 0,
  PANEL_COLORADJUST,
  PANEL_RASTERSETTINGS,
  PANEL_SEPARATOR1,
  PANEL_INPUTCONFIG,
  PANEL_SEPARATOR2,
  PANEL_CHEATEDITOR,
};

class SettingsWindow : public Window {
public:
Listbox Panel;

  bool Event(EventInfo &info);

  void Show();
  void Hide();
  void Setup();
} wSettings;
