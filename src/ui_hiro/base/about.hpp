class AboutWindow : public Window {
public:
  Canvas icon;
  Label about;
  Label localization;

  void setup();
  uintptr_t close(event_t);
} window_about;
