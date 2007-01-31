class MainWindow : public ui::Window { public:
ui::MenuGroup menu_file;
  ui::MenuItem menu_file_load_normal;
  ui::MenuItem menu_file_unload;
  //
  ui::MenuItem menu_file_exit;
ui::MenuGroup menu_settings;
  ui::MenuCheckItem menu_settings_mute;
  //
  ui::MenuItem menu_settings_config;
ui::MenuGroup menu_misc;
  ui::MenuItem menu_misc_about;

ui::Canvas canvas;
//
  bool close();
  void setup();
  void clicked(ui::Control *id);
} window_main;
