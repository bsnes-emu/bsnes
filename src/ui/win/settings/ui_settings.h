enum {
  PANEL_VIDEOSETTINGS = 0,
  PANEL_COLORADJUST,
  PANEL_RASTERSETTINGS,
  PANEL_EMUSETTINGS,
  PANEL_INPUTCONFIG,
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
