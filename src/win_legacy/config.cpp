namespace config {

struct System {
  static Setting regulate_speed;
} system;
Setting System::regulate_speed(&config_file, "system.regulate_speed", "Regulate speed to 60hz (NTSC) / 50hz (PAL)", true, Setting::TRUE_FALSE);

struct Video {
  static Setting mode, use_vram, vblank;
} video;
Setting Video::mode(&config_file, "video.mode",
  "Video mode\n"
  "  0 = 256x224w\n"
  "  1 = 512x448w\n"
  "  2 = 960x720w\n"
  "  3 = 640x480f\n"
  "  4 = 1024x768f",
  1, Setting::DEC);
Setting Video::use_vram(&config_file, "video.use_vram", "Use Video RAM instead of System RAM", true, Setting::TRUE_FALSE);
Setting Video::vblank(&config_file, "video.vblank", "Wait for vertical retrace when updating screen", false, Setting::TRUE_FALSE);

struct GUI {
  static Setting show_fps;
} gui;
Setting GUI::show_fps(&config_file, "gui.show_fps", "Show framerate in window title", true, Setting::TRUE_FALSE);

struct Input {
  struct Joypad1 {
    static Setting up, down, left, right, a, b, x, y, l, r, select, start;
  } joypad1;
  struct Joypad2 {
    static Setting up, down, left, right, a, b, x, y, l, r, select, start;
  } joypad2;
} input;
Setting Input::Joypad1::up    (&config_file, "input.joypad1.up",     "Joypad1 up",     VK_UP,     Setting::HEX);
Setting Input::Joypad1::down  (&config_file, "input.joypad1.down",   "Joypad1 down",   VK_DOWN,   Setting::HEX);
Setting Input::Joypad1::left  (&config_file, "input.joypad1.left",   "Joypad1 left",   VK_LEFT,   Setting::HEX);
Setting Input::Joypad1::right (&config_file, "input.joypad1.right",  "Joypad1 right",  VK_RIGHT,  Setting::HEX);
Setting Input::Joypad1::a     (&config_file, "input.joypad1.a",      "Joypad1 A",      'X',       Setting::HEX);
Setting Input::Joypad1::b     (&config_file, "input.joypad1.b",      "Joypad1 B",      'Z',       Setting::HEX);
Setting Input::Joypad1::x     (&config_file, "input.joypad1.x",      "Joypad1 X",      'S',       Setting::HEX);
Setting Input::Joypad1::y     (&config_file, "input.joypad1.y",      "Joypad1 Y",      'A',       Setting::HEX);
Setting Input::Joypad1::l     (&config_file, "input.joypad1.l",      "Joypad1 L",      'D',       Setting::HEX);
Setting Input::Joypad1::r     (&config_file, "input.joypad1.r",      "Joypad1 R",      'C',       Setting::HEX);
Setting Input::Joypad1::select(&config_file, "input.joypad1.select", "Joypad1 select", VK_SHIFT,  Setting::HEX);
Setting Input::Joypad1::start (&config_file, "input.joypad1.start",  "Joypad1 start",  VK_RETURN, Setting::HEX);

Setting Input::Joypad2::up    (&config_file, "input.joypad2.up",     "Joypad2 up",     'T', Setting::HEX);
Setting Input::Joypad2::down  (&config_file, "input.joypad2.down",   "Joypad2 down",   'G', Setting::HEX);
Setting Input::Joypad2::left  (&config_file, "input.joypad2.left",   "Joypad2 left",   'F', Setting::HEX);
Setting Input::Joypad2::right (&config_file, "input.joypad2.right",  "Joypad2 right",  'H', Setting::HEX);
Setting Input::Joypad2::a     (&config_file, "input.joypad2.a",      "Joypad2 A",      'K', Setting::HEX);
Setting Input::Joypad2::b     (&config_file, "input.joypad2.b",      "Joypad2 B",      'J', Setting::HEX);
Setting Input::Joypad2::x     (&config_file, "input.joypad2.x",      "Joypad2 X",      'I', Setting::HEX);
Setting Input::Joypad2::y     (&config_file, "input.joypad2.y",      "Joypad2 Y",      'U', Setting::HEX);
Setting Input::Joypad2::l     (&config_file, "input.joypad2.l",      "Joypad2 L",      'O', Setting::HEX);
Setting Input::Joypad2::r     (&config_file, "input.joypad2.r",      "Joypad2 R",      'L', Setting::HEX);
Setting Input::Joypad2::select(&config_file, "input.joypad2.select", "Joypad2 select", '[', Setting::HEX);
Setting Input::Joypad2::start (&config_file, "input.joypad2.start",  "Joypad2 start",  ']', Setting::HEX);
};
