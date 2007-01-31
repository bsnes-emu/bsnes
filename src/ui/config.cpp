namespace config {

struct System {
  static Setting video, audio, input;

  static Setting regulate_speed;
  static Setting speed_slowest, speed_slow, speed_normal, speed_fast, speed_fastest;
} system;
Setting System::video(&config_file, "system.video", "Video hardware interface", "");
Setting System::audio(&config_file, "system.audio", "Audio hardware interface", "");
Setting System::input(&config_file, "system.input", "Input hardware interface", "");

Setting System::regulate_speed(&config_file, "system.regulate_speed", "Regulate speed to 60hz (NTSC) / 50hz (PAL)", true, Setting::TRUE_FALSE);
Setting System::speed_slowest (&config_file, "system.speed_slowest",  "Slowest speed setting (in hz)", 16000, Setting::DEC);
Setting System::speed_slow    (&config_file, "system.speed_slow",     "Slow speed setting",            24000, Setting::DEC);
Setting System::speed_normal  (&config_file, "system.speed_normal",   "Normal speed setting",          32000, Setting::DEC);
Setting System::speed_fast    (&config_file, "system.speed_fast",     "Fast speed setting",            48000, Setting::DEC);
Setting System::speed_fastest (&config_file, "system.speed_fastest",  "Fastest speed setting",         64000, Setting::DEC);

struct Video {
  static Setting profile;
  static Setting profile_win, profile_full;

  static Setting use_vram;
  static Setting pscanline_intensity, iscanline_intensity;
} video;
Setting Video::profile(0, "video.profile", "", 0, Setting::DEC);

/* software_filter
 * hardware_filter
 * video_standard
 * multiplier
 * correct_aspect_ratio
 * enable_scanlines
 * manual_render_size
 * render_width
 * render_height
 * fullscreen
 * resolution_width
 * resolution_height
 * refresh_rate
 * triple_buffering
 */
Setting Video::profile_win(&config_file, "video.profile_win", "Windowed video profile configuration\n"
  "If available, please use bsnes GUI configuration editor to modify video profile settings\n"
  "Format: software_filter;hardware_filter;video_standard;multiplier;correct_aspect_ratio;\n"
  "        enable_scanlines;manual_render_size;render_width;render_height;\n"
  "        triple_buffering;resolution_width;resolution_height;refresh_rate",
  "0;1;0;2;true;false;false;595;448;false;0;0;0");
Setting Video::profile_full(&config_file, "video.profile_full", "Fullscreen video profile configuration",
  "0;1;0;2;true;false;false;595;448;false;0;0;0");

Setting Video::use_vram(&config_file, "video.use_vram", "Use Video RAM instead of System RAM", true, Setting::TRUE_FALSE);
Setting Video::pscanline_intensity(&config_file, "video.pscanline_intensity",
  "Progressive scanline intensity\n"
  "Value is percentage of intensity from 0 to 100", 30, Setting::DEC);
Setting Video::iscanline_intensity(&config_file, "video.iscanline_intensity",
  "Interlace scanline intensity", 50, Setting::DEC);

struct Audio {
  static Setting frequency;
  static Setting latency;
} audio;
Setting Audio::frequency(&config_file, "audio.frequency", "Default audio playback frequency.", 32000, Setting::DEC);
Setting Audio::latency(&config_file, "audio.latency", "Audio playback latency in milliseconds.\n"
  "Specifies how long audio playback is delayed compared to a real SNES.\n"
  "A delay is necessary to allow smooth audio playback via buffering.\n"
  "Raising this value may help with audio playback problems, but will decrease\n"
  "audio responsiveness.",
  75, Setting::DEC);

struct Input {
  static Setting axis_resistance;
  struct Joypad1 {
    static Setting allow_invalid_input;
    static Setting up, down, left, right, a, b, x, y, l, r, select, start;
    static Setting map;
  } joypad1;
  struct Joypad2 {
    static Setting allow_invalid_input;
    static Setting up, down, left, right, a, b, x, y, l, r, select, start;
    static Setting map;
  } joypad2;
} input;

Setting Input::axis_resistance(&config_file, "input.axis_resistance",
  "Axis resistance for all analog joypads\n"
  "Affects responsiveness of analog stick movement by specifying what percentage\n"
  "in any given direction the axis must be pressed to trigger a button press.\n"
  "In other words, this determines how hard you have to press the analog stick to\n"
  "simulate pressing e.g. left or right on a digital joypad.\n"
  "Value is a percentage, from 0 (axis will trigger with virtually any axis movement)\n"
  "up to 100 (axis must be pressed fully to given corner).\n"
  "Value affects all four directions of the axis equally.\n"
  "Note: Values below 10 or above 90 are not recommended and may not work at all.",
  75, Setting::DEC);

Setting Input::Joypad1::allow_invalid_input(&config_file, "input.joypad1.allow_invalid_input",
  "Allow up+down and left+right key combinations for joypad 1 (not recommended)", false, Setting::TRUE_FALSE);

Setting Input::Joypad1::up    (0, "input.joypad1.up",     "", 0, Setting::HEX);
Setting Input::Joypad1::down  (0, "input.joypad1.down",   "", 0, Setting::HEX);
Setting Input::Joypad1::left  (0, "input.joypad1.left",   "", 0, Setting::HEX);
Setting Input::Joypad1::right (0, "input.joypad1.right",  "", 0, Setting::HEX);
Setting Input::Joypad1::a     (0, "input.joypad1.a",      "", 0, Setting::HEX);
Setting Input::Joypad1::b     (0, "input.joypad1.b",      "", 0, Setting::HEX);
Setting Input::Joypad1::x     (0, "input.joypad1.x",      "", 0, Setting::HEX);
Setting Input::Joypad1::y     (0, "input.joypad1.y",      "", 0, Setting::HEX);
Setting Input::Joypad1::l     (0, "input.joypad1.l",      "", 0, Setting::HEX);
Setting Input::Joypad1::r     (0, "input.joypad1.r",      "", 0, Setting::HEX);
Setting Input::Joypad1::select(0, "input.joypad1.select", "", 0, Setting::HEX);
Setting Input::Joypad1::start (0, "input.joypad1.start",  "", 0, Setting::HEX);

Setting Input::Joypad1::map(&config_file, "input.joypad1.map", "Joypad 1 button map\n"
  "Format: Up; Down; Left; Right; A; B; X; Y; L; R; Select; Start",
  "up | joypad0.up; "       "down | joypad0.down; "       "left | joypad0.left; "     "right | joypad0.right; "
   "x | joypad0.button3; "     "z | joypad0.button2; "       "s | joypad0.button1; "      "a | joypad0.button0; "
   "d | joypad0.button6; "     "c | joypad0.button7; "  "rshift | joypad0.button4; "  "enter | joypad0.button5");

//

Setting Input::Joypad2::allow_invalid_input(&config_file, "input.joypad2.allow_invalid_input",
  "Allow up+down and left+right key combinations for joypad 2 (not recommended)", false, Setting::TRUE_FALSE);

Setting Input::Joypad2::up    (0, "input.joypad2.up",     "", 0, Setting::HEX);
Setting Input::Joypad2::down  (0, "input.joypad2.down",   "", 0, Setting::HEX);
Setting Input::Joypad2::left  (0, "input.joypad2.left",   "", 0, Setting::HEX);
Setting Input::Joypad2::right (0, "input.joypad2.right",  "", 0, Setting::HEX);
Setting Input::Joypad2::a     (0, "input.joypad2.a",      "", 0, Setting::HEX);
Setting Input::Joypad2::b     (0, "input.joypad2.b",      "", 0, Setting::HEX);
Setting Input::Joypad2::x     (0, "input.joypad2.x",      "", 0, Setting::HEX);
Setting Input::Joypad2::y     (0, "input.joypad2.y",      "", 0, Setting::HEX);
Setting Input::Joypad2::l     (0, "input.joypad2.l",      "", 0, Setting::HEX);
Setting Input::Joypad2::r     (0, "input.joypad2.r",      "", 0, Setting::HEX);
Setting Input::Joypad2::select(0, "input.joypad2.select", "", 0, Setting::HEX);
Setting Input::Joypad2::start (0, "input.joypad2.start",  "", 0, Setting::HEX);

Setting Input::Joypad2::map(&config_file, "input.joypad2.map", "Joypad 2 button map\n"
  "Format: Up; Down; Left; Right; A; B; X; Y; L; R; Select; Start",
  "t | joypad1.up; "       "g | joypad1.down; "            "f | joypad1.left; "            "h | joypad1.right; "
  "k | joypad1.button3; "  "j | joypad1.button2; "         "i | joypad1.button1; "         "u | joypad1.button0; "
  "o | joypad1.button6; "  "l | joypad1.button7; "  "lbracket | joypad1.button4; "  "rbracket | joypad1.button5");

};
