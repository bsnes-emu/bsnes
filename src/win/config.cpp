namespace config {

struct System {
  static Setting regulate_speed;
  static Setting speed_slowest, speed_slow, speed_normal, speed_fast, speed_fastest;
} system;
Setting System::regulate_speed(&config_file, "system.regulate_speed", "Regulate speed to 60hz (NTSC) / 50hz (PAL)", true, Setting::TRUE_FALSE);
Setting System::speed_slowest (&config_file, "system.speed_slowest",  "Slowest speed setting (in hz)", 16000, Setting::DEC);
Setting System::speed_slow    (&config_file, "system.speed_slow",     "Slow speed setting",            24000, Setting::DEC);
Setting System::speed_normal  (&config_file, "system.speed_normal",   "Normal speed setting",          32000, Setting::DEC);
Setting System::speed_fast    (&config_file, "system.speed_fast",     "Fast speed setting",            48000, Setting::DEC);
Setting System::speed_fastest (&config_file, "system.speed_fastest",  "Fastest speed setting",         64000, Setting::DEC);

struct Video {
  static Setting renderer;
  static Setting profile;
  static Setting profile_windowed_default, profile_fullscreen_default;
  static Setting profile_0, profile_1, profile_2, profile_3, profile_4;
  static Setting profile_5, profile_6, profile_7, profile_8, profile_9;

  static Setting use_vram, triple_buffering;
  static Setting pscanline_intensity, iscanline_intensity;

  struct Filter {
    static Setting software, hardware;
  } filter;
} video;
Setting Video::renderer(&config_file, "video.renderer", "Video renderer\n"
  "\"dd\" (DirectDraw7 -- faster, less features)\n"
  "\"d3d\" (Direct3D9 -- slower, more features)", "d3d");
Setting Video::profile(&config_file, "video.profile", "Video profile to load at startup", 2, Setting::DEC);
Setting Video::profile_windowed_default(&config_file, "video.profile_windowed_default",
  "Windowed profile to select for Alt+Enter", 0, Setting::DEC);
Setting Video::profile_fullscreen_default(&config_file, "video.profile_fullscreen_default",
  "Fullscreen profile to select for Alt+Enter", 5, Setting::DEC);

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
Setting Video::profile_0(&config_file, "video.profile_0", "Video profile 0 configuration\n"
  "Please use bsnes GUI configuration editor to modify video profile settings\n"
  "Format: software_filter;hardware_filter;video_standard;multiplier;correct_aspect_ratio;\n"
  "        enable_scanlines;manual_render_size;render_width;render_height;fullscreen;\n"
  "        resolution_width;resolution_height;refresh_rate;triple_buffering"
  "",                                                   "0;0;0;0;false;false;false;256;224;false;640;480;60;false");
Setting Video::profile_1(&config_file, "video.profile_1", "", "0;0;0;0;false;false;false;256;224;false;640;480;60;false");
Setting Video::profile_2(&config_file, "video.profile_2", "", "0;0;0;0;false;false;false;256;224;false;640;480;60;false");
Setting Video::profile_3(&config_file, "video.profile_3", "", "0;0;0;0;false;false;false;256;224;false;640;480;60;false");
Setting Video::profile_4(&config_file, "video.profile_4", "", "0;0;0;0;false;false;false;256;224;false;640;480;60;false");
Setting Video::profile_5(&config_file, "video.profile_5", "", "0;0;0;0;false;false;false;256;224;false;640;480;60;false");
Setting Video::profile_6(&config_file, "video.profile_6", "", "0;0;0;0;false;false;false;256;224;false;640;480;60;false");
Setting Video::profile_7(&config_file, "video.profile_7", "", "0;0;0;0;false;false;false;256;224;false;640;480;60;false");
Setting Video::profile_8(&config_file, "video.profile_8", "", "0;0;0;0;false;false;false;256;224;false;640;480;60;false");
Setting Video::profile_9(&config_file, "video.profile_9", "", "0;0;0;0;false;false;false;256;224;false;640;480;60;false");

Setting Video::use_vram(&config_file, "video.use_vram", "Use Video RAM instead of System RAM", true, Setting::TRUE_FALSE);
Setting Video::triple_buffering(&config_file, "video.triple_buffering", "Use triple buffering", false, Setting::TRUE_FALSE);
Setting Video::pscanline_intensity(&config_file, "video.pscanline_intensity",
  "Progressive scanline intensity\n"
  "Value is percentage of intensity from 0 to 100", 30, Setting::DEC);
Setting Video::iscanline_intensity(&config_file, "video.iscanline_intensity",
  "Interlace scanline intensity", 50, Setting::DEC);

Setting Video::Filter::software(&config_file, "video.filter.software", "Software video filter\n"
  "0 = Direct\n"
  "1 = Scale2x\n", 0, Setting::DEC);

Setting Video::Filter::hardware(&config_file, "video.filter.hardware", "Hardware video filter\n"
  "0 = Pixel\n"
  "1 = Bilinear", 1, Setting::DEC);

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
  "up | joy0.up; down | joy0.down; left | joy0.left; right | joy0.right; "
  "x | joy0.button4; z | joy0.button3; s | joy0.button1; a | joy0.button0; "
  "d | joy0.button6; c | joy0.button7; rshift | joy0.button2; enter | joy0.button5");

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
  "t | joy1.up; g | joy1.down; f | joy1.left; h | joy1.right; "
  "k | joy1.button4; j | joy1.button3; i | joy1.button1; u | joy1.button0; "
  "o | joy1.button6; l | joy1.button7; lbracket | joy1.button2; rbracket | joy1.button5");

//

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
  192, Setting::DEC);

};
