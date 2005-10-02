namespace config {

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
  struct Joypad {
    static Setting up, down, left, right, a, b, x, y, l, r, select, start;
  } joypad1;
} input;
Setting Input::Joypad::up    (&config_file, "input.joypad1.up",     "Joypad1 up",     VK_UP,     Setting::HEX);
Setting Input::Joypad::down  (&config_file, "input.joypad1.down",   "Joypad1 down",   VK_DOWN,   Setting::HEX);
Setting Input::Joypad::left  (&config_file, "input.joypad1.left",   "Joypad1 left",   VK_LEFT,   Setting::HEX);
Setting Input::Joypad::right (&config_file, "input.joypad1.right",  "Joypad1 right",  VK_RIGHT,  Setting::HEX);
Setting Input::Joypad::a     (&config_file, "input.joypad1.a",      "Joypad1 A",      'X',       Setting::HEX);
Setting Input::Joypad::b     (&config_file, "input.joypad1.b",      "Joypad1 B",      'Z',       Setting::HEX);
Setting Input::Joypad::x     (&config_file, "input.joypad1.x",      "Joypad1 X",      'S',       Setting::HEX);
Setting Input::Joypad::y     (&config_file, "input.joypad1.y",      "Joypad1 Y",      'A',       Setting::HEX);
Setting Input::Joypad::l     (&config_file, "input.joypad1.l",      "Joypad1 L",      'D',       Setting::HEX);
Setting Input::Joypad::r     (&config_file, "input.joypad1.r",      "Joypad1 R",      'C',       Setting::HEX);
Setting Input::Joypad::select(&config_file, "input.joypad1.select", "Joypad1 select", VK_SHIFT,  Setting::HEX);
Setting Input::Joypad::start (&config_file, "input.joypad1.start",  "Joypad1 start",  VK_RETURN, Setting::HEX);

};
