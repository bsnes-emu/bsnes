struct Settings : Markup::Node {
  Settings();
  auto save() -> void;
};

struct SettingsWindow : Window {
  SettingsWindow();

  VerticalLayout layout{this};
    TabFrame panel{&layout, Size{~0, ~0}};
      TabFrameItem placeholder{&panel};
};

extern Settings settings;
extern unique_pointer<SettingsWindow> settingsWindow;
