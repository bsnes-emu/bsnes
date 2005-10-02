namespace config {

struct Video {
  static Setting fullscreen;
  static Setting display_width, display_height;
  static Setting output_width,  output_height;
} video;
Setting Video::fullscreen(&config_file, "video.fullscreen", "Enable fullscreen mode at startup", false, Setting::TRUE_FALSE);
Setting Video::display_width (&config_file, "video.display_width",  "Window / Fullscreen width",  320, Setting::DEC);
Setting Video::display_height(&config_file, "video.display_height", "Window / Fullscreen height", 240, Setting::DEC);
Setting Video::output_width  (&config_file, "video.output_width",   "SNES video output width",    256, Setting::DEC);
Setting Video::output_height (&config_file, "video.output_height",  "SNES video output height",   223, Setting::DEC);

struct Input {
  struct Joypad {
    static Setting up, down, left, right, a, b, x, y, l, r, select, start;
  } joypad1;
} input;
Setting Input::Joypad::up    (&config_file, "input.joypad1.up",     "Joypad1 up",     SDLK_UP,     Setting::HEX);
Setting Input::Joypad::down  (&config_file, "input.joypad1.down",   "Joypad1 down",   SDLK_DOWN,   Setting::HEX);
Setting Input::Joypad::left  (&config_file, "input.joypad1.left",   "Joypad1 left",   SDLK_LEFT,   Setting::HEX);
Setting Input::Joypad::right (&config_file, "input.joypad1.right",  "Joypad1 right",  SDLK_RIGHT,  Setting::HEX);
Setting Input::Joypad::a     (&config_file, "input.joypad1.a",      "Joypad1 A",      SDLK_x,      Setting::HEX);
Setting Input::Joypad::b     (&config_file, "input.joypad1.b",      "Joypad1 B",      SDLK_z,      Setting::HEX);
Setting Input::Joypad::x     (&config_file, "input.joypad1.x",      "Joypad1 X",      SDLK_s,      Setting::HEX);
Setting Input::Joypad::y     (&config_file, "input.joypad1.y",      "Joypad1 Y",      SDLK_a,      Setting::HEX);
Setting Input::Joypad::l     (&config_file, "input.joypad1.l",      "Joypad1 L",      SDLK_d,      Setting::HEX);
Setting Input::Joypad::r     (&config_file, "input.joypad1.r",      "Joypad1 R",      SDLK_c,      Setting::HEX);
Setting Input::Joypad::select(&config_file, "input.joypad1.select", "Joypad1 select", SDLK_RSHIFT, Setting::HEX);
Setting Input::Joypad::start (&config_file, "input.joypad1.start",  "Joypad1 start",  SDLK_RETURN, Setting::HEX);

};
