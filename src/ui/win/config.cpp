namespace config {

struct Debugger {
  static Setting console_lines;
} debugger;
Setting Debugger::console_lines(&config_file, "debugger.console_lines", "Number of lines buffered for debugger console",
  100, Setting::DEC);

struct Misc {
  static Setting image_format;
  static Setting window_style;
  static Setting show_fps;
  static Setting config_window_alpha_level;
} misc;
Setting Misc::image_format(&config_file, "misc.image_format", "Image format for screenshots\n"
  "Valid formats: \"bmp\", \"png\", \"jpg\"", "png");
Setting Misc::window_style(&config_file, "misc.window_style", "Window style for main emulation window",
  "titlebar|frame|minimize|dragmove");
Setting Misc::show_fps(&config_file, "misc.show_fps", "Show framerate", true, Setting::TRUE_FALSE);
Setting Misc::config_window_alpha_level(&config_file, "misc.config_window_alpha_level",
  "Alpha level (opacity) of configuration window\n"
  "Value must be between 64 (25% opaque, 75% transparent) and 255 (100% opaque)",
  255, Setting::DEC);

};
