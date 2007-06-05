class AboutWindow : public ui::Window { public:
ui::Label about;
static const char about_text[4096];
  bool message(uint id, uintptr_t param);
  void setup();
} window_about;
