class AboutWindow : public ui::Window { public:
ui::Editbox about;
static const char about_text[4096];
  bool close();
  void setup();
} window_about;
