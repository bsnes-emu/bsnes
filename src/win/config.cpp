namespace config {

struct System {
  static Setting regulate_speed;
  static Setting speed_slowest, speed_slow, speed_normal, speed_fast, speed_fastest;
} system;
Setting System::regulate_speed(&config_file, "system.regulate_speed", "Regulate speed to 60hz (NTSC) / 50hz (PAL)", true, Setting::TRUE_FALSE);
Setting System::speed_slowest(&config_file, "system.speed_slowest", "Slowest speed setting (in hz)", 16000, Setting::DEC);
Setting System::speed_slow   (&config_file, "system.speed_slow",    "Slow speed setting",            24000, Setting::DEC);
Setting System::speed_normal (&config_file, "system.speed_normal",  "Normal speed setting",          32000, Setting::DEC);
Setting System::speed_fast   (&config_file, "system.speed_fast",    "Fast speed setting",            48000, Setting::DEC);
Setting System::speed_fastest(&config_file, "system.speed_fastest", "Fastest speed setting",         64000, Setting::DEC);

struct Video {
  static Setting mode;
  static Setting mode0, mode1, mode2, mode3, mode4;
  static Setting mode5, mode6, mode7, mode8, mode9;
  static Setting use_vram, triple_buffering;
} video;
Setting Video::mode(&config_file, "video.mode", "Video mode at startup", 2, Setting::DEC);
Setting Video::mode0(&config_file, "video.mode_0", "Video mode 0 (windowed)",   "256x223");
Setting Video::mode1(&config_file, "video.mode_1", "Video mode 1 (windowed)",   "512x446");
Setting Video::mode2(&config_file, "video.mode_2", "Video mode 2 (windowed)",   "640x480");
Setting Video::mode3(&config_file, "video.mode_3", "Video mode 3 (windowed)",   "960x720");
Setting Video::mode4(&config_file, "video.mode_4", "Video mode 4 (windowed)",   "1152x864");
Setting Video::mode5(&config_file, "video.mode_5", "Video mode 5 (fullscreen)", "640x480@60:640x480");
Setting Video::mode6(&config_file, "video.mode_6", "Video mode 6 (fullscreen)", "800x600@60:800x600");
Setting Video::mode7(&config_file, "video.mode_7", "Video mode 7 (fullscreen)", "1024x768@60:1024x768");
Setting Video::mode8(&config_file, "video.mode_8", "Video mode 8 (fullscreen)", "1280x960@60:1280x960");
Setting Video::mode9(&config_file, "video.mode_9", "Video mode 9 (fullscreen)", "1600x1200@60:1600x1200");
Setting Video::use_vram(&config_file, "video.use_vram", "Use Video RAM instead of System RAM", true, Setting::TRUE_FALSE);
Setting Video::triple_buffering(&config_file, "video.triple_buffering", "Use triple buffering", false, Setting::TRUE_FALSE);

struct GUI {
  static Setting show_fps;
} gui;
Setting GUI::show_fps(&config_file, "gui.show_fps", "Show framerate in window title", true, Setting::TRUE_FALSE);

struct Input {
  struct Joypad1 {
    static Setting allow_invalid_input;
    static Setting up, down, left, right, a, b, x, y, l, r, select, start;
  } joypad1;
  struct Joypad2 {
    static Setting allow_invalid_input;
    static Setting up, down, left, right, a, b, x, y, l, r, select, start;
  } joypad2;
} input;

Setting Input::Joypad1::allow_invalid_input(&config_file, "input.joypad1.allow_invalid_input",
  "Allow \"impossible\" key combinations for joypad 1 (not recommended)", false, Setting::TRUE_FALSE);
Setting Input::Joypad1::up    (&config_file, "input.joypad1.up",     "Joypad1 up",     0x80c8, Setting::HEX);
Setting Input::Joypad1::down  (&config_file, "input.joypad1.down",   "Joypad1 down",   0x81d0, Setting::HEX);
Setting Input::Joypad1::left  (&config_file, "input.joypad1.left",   "Joypad1 left",   0x82cb, Setting::HEX);
Setting Input::Joypad1::right (&config_file, "input.joypad1.right",  "Joypad1 right",  0x83cd, Setting::HEX);
Setting Input::Joypad1::a     (&config_file, "input.joypad1.a",      "Joypad1 A",      0x042d, Setting::HEX);
Setting Input::Joypad1::b     (&config_file, "input.joypad1.b",      "Joypad1 B",      0x032c, Setting::HEX);
Setting Input::Joypad1::x     (&config_file, "input.joypad1.x",      "Joypad1 X",      0x011f, Setting::HEX);
Setting Input::Joypad1::y     (&config_file, "input.joypad1.y",      "Joypad1 Y",      0x001e, Setting::HEX);
Setting Input::Joypad1::l     (&config_file, "input.joypad1.l",      "Joypad1 L",      0x0620, Setting::HEX);
Setting Input::Joypad1::r     (&config_file, "input.joypad1.r",      "Joypad1 R",      0x072e, Setting::HEX);
Setting Input::Joypad1::select(&config_file, "input.joypad1.select", "Joypad1 select", 0x0836, Setting::HEX);
Setting Input::Joypad1::start (&config_file, "input.joypad1.start",  "Joypad1 start",  0x091c, Setting::HEX);

Setting Input::Joypad2::allow_invalid_input(&config_file, "input.joypad2.allow_invalid_input",
  "Allow \"impossible\" key combinations for joypad 2 (not recommended)", false, Setting::TRUE_FALSE);
Setting Input::Joypad2::up    (&config_file, "input.joypad2.up",     "Joypad2 up",     0xff14, Setting::HEX);
Setting Input::Joypad2::down  (&config_file, "input.joypad2.down",   "Joypad2 down",   0xff22, Setting::HEX);
Setting Input::Joypad2::left  (&config_file, "input.joypad2.left",   "Joypad2 left",   0xff21, Setting::HEX);
Setting Input::Joypad2::right (&config_file, "input.joypad2.right",  "Joypad2 right",  0xff23, Setting::HEX);
Setting Input::Joypad2::a     (&config_file, "input.joypad2.a",      "Joypad2 A",      0xff25, Setting::HEX);
Setting Input::Joypad2::b     (&config_file, "input.joypad2.b",      "Joypad2 B",      0xff24, Setting::HEX);
Setting Input::Joypad2::x     (&config_file, "input.joypad2.x",      "Joypad2 X",      0xff17, Setting::HEX);
Setting Input::Joypad2::y     (&config_file, "input.joypad2.y",      "Joypad2 Y",      0xff16, Setting::HEX);
Setting Input::Joypad2::l     (&config_file, "input.joypad2.l",      "Joypad2 L",      0xff18, Setting::HEX);
Setting Input::Joypad2::r     (&config_file, "input.joypad2.r",      "Joypad2 R",      0xff26, Setting::HEX);
Setting Input::Joypad2::select(&config_file, "input.joypad2.select", "Joypad2 select", 0xff1a, Setting::HEX);
Setting Input::Joypad2::start (&config_file, "input.joypad2.start",  "Joypad2 start",  0xff1b, Setting::HEX);

};
