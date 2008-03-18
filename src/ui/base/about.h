class AboutWindow : public Window {
public:
  Canvas icon;
  Label about;
  Label contributors;
  static const char about_text[1024];
  static const char contributors_text[1024];

  void setup();
  uintptr_t close(Event);
} window_about;
