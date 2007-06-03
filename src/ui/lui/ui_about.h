class AboutWindow : public ui::Window { public:
ui::Label about;
static const char about_text[4096];
  int  message(uint id, void *param);
  void setup();
} window_about;
