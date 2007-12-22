class AboutWindow : public Window {
public:
  Label about;
  static const char about_text[4096];

  void setup();
  uintptr_t close(Event);
} window_about;
