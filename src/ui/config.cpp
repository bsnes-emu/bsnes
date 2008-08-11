namespace config {

char bsnes_cfg[PATH_MAX] = "";
char locale_cfg[PATH_MAX] = "";

struct System {
  static string_setting video, audio, input;
  static integral_setting emulation_speed;
  static integral_setting gamma_ramp, sepia, grayscale, invert, contrast, brightness, gamma;
} system;

string_setting System::video(config(), "system.video", "Video hardware interface", "");
string_setting System::audio(config(), "system.audio", "Audio hardware interface", "");
string_setting System::input(config(), "system.input", "Input hardware interface", "");

integral_setting System::emulation_speed(config(), "system.emulation_speed",
  "Relative speed of emulator compared to SNES hardware\n"
  "0 = 50%\n"
  "1 = 75%\n"
  "2 = 100%\n"
  "3 = 150%\n"
  "4 = 200%\n"
  "5 = Uncapped",
  integral_setting::decimal, 2);

integral_setting System::gamma_ramp(config(), "system.colorfilter.gamma_ramp",
  "Use precalculated TV-style gamma ramp", integral_setting::boolean, true);
integral_setting System::sepia(config(), "system.colorfilter.sepia",
  "Convert color to sepia tone", integral_setting::boolean, false);
integral_setting System::grayscale(config(), "system.colorfilter.grayscale",
  "Convert color to grayscale tone", integral_setting::boolean, false);
integral_setting System::invert(config(), "system.colorfilter.invert",
  "Invert output image colors", integral_setting::boolean, false);
integral_setting System::contrast(config(), "system.colorfilter.contrast",
  "Contrast", integral_setting::decimal, 0);
integral_setting System::brightness(config(), "system.colorfilter.brightness",
  "Brightness", integral_setting::decimal, 0);
integral_setting System::gamma(config(), "system.colorfilter.gamma",
  "Gamma", integral_setting::decimal, 80);

struct Video {
  static integral_setting mode;
  struct Windowed {
    static integral_setting synchronize, aspect_correction;
    static integral_setting region, multiplier, hardware_filter, software_filter;
  } windowed;
  struct Fullscreen {
    static integral_setting synchronize, aspect_correction;
    static integral_setting region, multiplier, hardware_filter, software_filter;
  } fullscreen;
  static integral_setting aspect_ntsc_x, aspect_ntsc_y, aspect_pal_x, aspect_pal_y;
  static integral_setting frameskip;
} video;

//0 = windowed, 1 = fullscreen, 2 = exclusive
integral_setting Video::mode("video.mode", "Active video mode", integral_setting::decimal, 0);

integral_setting Video::Windowed::synchronize(config(), "video.windowed.synchronize", "Synchronize to video refresh rate", integral_setting::boolean, false);
integral_setting Video::Windowed::aspect_correction(config(), "video.windowed.aspect_correction",
  "Correct video aspect ratio\n"
  "Defaults assume display pixels are perfectly square\n"
  "Formula: width = width * video.aspect_<region>_x / video.aspect_<region>_y\n",
  integral_setting::boolean, true);
integral_setting Video::Windowed::region(config(), "video.windowed.region", "Video output region\n"
  "0 = NTSC, 1 = PAL",
  integral_setting::decimal, 0);
integral_setting Video::Windowed::multiplier(config(), "video.windowed.multiplier", "Video output size multiplier (1-5x)\n"
  "1 = 1x (<= 320x240)\n"
  "2 = 2x (<= 640x480)\n"
  "etc.",
  integral_setting::decimal, 2);
integral_setting Video::Windowed::hardware_filter(config(), "video.windowed.hardware_filter", "Video hardware filter\n"
  "0 = Point\n"
  "1 = Linear\n",
  integral_setting::decimal, 1);
integral_setting Video::Windowed::software_filter(config(), "video.windowed.software_filter", "Video software filter\n"
  "0 = None\n"
  "1 = Scanline\n"
  "2 = Scale2x\n"
  "3 = HQ2x\n"
  "4 = NTSC\n",
  integral_setting::decimal, 0);

integral_setting Video::Fullscreen::synchronize      (config(), "video.fullscreen.synchronize",       "", integral_setting::boolean, false);
integral_setting Video::Fullscreen::aspect_correction(config(), "video.fullscreen.aspect_correction", "", integral_setting::boolean, true);
integral_setting Video::Fullscreen::region           (config(), "video.fullscreen.region",            "", integral_setting::decimal, 0);
integral_setting Video::Fullscreen::multiplier       (config(), "video.fullscreen.multiplier",        "", integral_setting::decimal, 2);
integral_setting Video::Fullscreen::hardware_filter  (config(), "video.fullscreen.hardware_filter",   "", integral_setting::decimal, 1);
integral_setting Video::Fullscreen::software_filter  (config(), "video.fullscreen.software_filter",   "", integral_setting::decimal, 0);

integral_setting Video::aspect_ntsc_x(config(), "video.aspect_ntsc_x", "", integral_setting::decimal, 54);
integral_setting Video::aspect_ntsc_y(config(), "video.aspect_ntsc_y", "", integral_setting::decimal, 47);
integral_setting Video::aspect_pal_x (config(), "video.aspect_pal_x",  "", integral_setting::decimal, 32);
integral_setting Video::aspect_pal_y (config(), "video.aspect_pal_y",  "", integral_setting::decimal, 23);

integral_setting Video::frameskip("video.frameskip", "Video frameskip", integral_setting::decimal, 0);

struct Audio {
  static integral_setting synchronize;
  static integral_setting mute;
} audio;
integral_setting Audio::synchronize(config(), "audio.synchronize", "Synchronize to audio sample rate", integral_setting::boolean, true);
integral_setting Audio::mute(config(), "audio.mute", "Mute audio playback", integral_setting::boolean, false);

struct Input {
  static integral_setting capture_mode;
  static integral_setting allow_invalid_input;
  static integral_setting analog_axis_resistance;

  struct Joypad1    { static string_setting up, down, left, right, a, b, x, y, l, r, select, start; } joypad1;
  struct Joypad2    { static string_setting up, down, left, right, a, b, x, y, l, r, select, start; } joypad2;

  struct Multitap1A { static string_setting up, down, left, right, a, b, x, y, l, r, select, start; } multitap1a;
  struct Multitap1B { static string_setting up, down, left, right, a, b, x, y, l, r, select, start; } multitap1b;
  struct Multitap1C { static string_setting up, down, left, right, a, b, x, y, l, r, select, start; } multitap1c;
  struct Multitap1D { static string_setting up, down, left, right, a, b, x, y, l, r, select, start; } multitap1d;
  struct Multitap2A { static string_setting up, down, left, right, a, b, x, y, l, r, select, start; } multitap2a;
  struct Multitap2B { static string_setting up, down, left, right, a, b, x, y, l, r, select, start; } multitap2b;
  struct Multitap2C { static string_setting up, down, left, right, a, b, x, y, l, r, select, start; } multitap2c;
  struct Multitap2D { static string_setting up, down, left, right, a, b, x, y, l, r, select, start; } multitap2d;

  struct GUI {
    static string_setting load;
    static string_setting pause;
    static string_setting reset;
    static string_setting power;
    static string_setting quit;
    static string_setting speed_decrease;
    static string_setting speed_increase;
    static string_setting frameskip_decrease;
    static string_setting frameskip_increase;
    static string_setting toggle_fullscreen;
    static string_setting toggle_menubar;
    static string_setting toggle_statusbar;
  } gui;
} input;

integral_setting Input::capture_mode(config(), "input.capture_mode",
  "Capture method for input to main emulation window\n"
  "When emulation window does not have focus:\n"
  "0 = Allow input\n"
  "1 = Ignore input\n"
  "2 = Pause emulator",
  integral_setting::decimal, 2);

integral_setting Input::allow_invalid_input(config(), "input.allow_invalid_input",
  "Allow up+down and left+right combinations (not recommended.)\n"
  "This is not possible on an actual SNES controller, due to its design.\n"
  "Enabling this option can trigger bugs in certain games.\n",
  integral_setting::boolean, false);

integral_setting Input::analog_axis_resistance(config(), "input.analog_axis_resistance",
  "Resistance required to activate analog stick in any given direction.\n"
  "This value ranges from 1 (1%, virtually no resistance) to 99 (99%, near full resistance.)\n"
  "For instance, a value of 50 means that to register 'left', the analog stick must be moved\n"
  "50% between the center and the left.\n"
  "Less resistance allows for more fluid movement; whereas more resistance helps to prevent\n"
  "accidental movements, eg attempting to press up whilst bumping the stick slightly left.",
  integral_setting::decimal, 50);

string_setting Input::Joypad1::up    (config(), "input.joypad1.up",     "", "up");
string_setting Input::Joypad1::down  (config(), "input.joypad1.down",   "", "down");
string_setting Input::Joypad1::left  (config(), "input.joypad1.left",   "", "left");
string_setting Input::Joypad1::right (config(), "input.joypad1.right",  "", "right");
string_setting Input::Joypad1::a     (config(), "input.joypad1.a",      "", "x");
string_setting Input::Joypad1::b     (config(), "input.joypad1.b",      "", "z");
string_setting Input::Joypad1::x     (config(), "input.joypad1.x",      "", "s");
string_setting Input::Joypad1::y     (config(), "input.joypad1.y",      "", "a");
string_setting Input::Joypad1::l     (config(), "input.joypad1.l",      "", "d");
string_setting Input::Joypad1::r     (config(), "input.joypad1.r",      "", "c");
string_setting Input::Joypad1::select(config(), "input.joypad1.select", "", "rshift");
string_setting Input::Joypad1::start (config(), "input.joypad1.start",  "", "return");

string_setting Input::Joypad2::up    (config(), "input.joypad2.up",     "", "t");
string_setting Input::Joypad2::down  (config(), "input.joypad2.down",   "", "g");
string_setting Input::Joypad2::left  (config(), "input.joypad2.left",   "", "f");
string_setting Input::Joypad2::right (config(), "input.joypad2.right",  "", "h");
string_setting Input::Joypad2::a     (config(), "input.joypad2.a",      "", "k");
string_setting Input::Joypad2::b     (config(), "input.joypad2.b",      "", "j");
string_setting Input::Joypad2::x     (config(), "input.joypad2.x",      "", "i");
string_setting Input::Joypad2::y     (config(), "input.joypad2.y",      "", "u");
string_setting Input::Joypad2::l     (config(), "input.joypad2.l",      "", "o");
string_setting Input::Joypad2::r     (config(), "input.joypad2.r",      "", "l");
string_setting Input::Joypad2::select(config(), "input.joypad2.select", "", "lbracket");
string_setting Input::Joypad2::start (config(), "input.joypad2.start",  "", "rbracket");

#define DeclMultitap(uname, lname) \
string_setting Input::uname::up    (config(), "input.multitap" lname ".up",     "", "none"); \
string_setting Input::uname::down  (config(), "input.multitap" lname ".down",   "", "none"); \
string_setting Input::uname::left  (config(), "input.multitap" lname ".left",   "", "none"); \
string_setting Input::uname::right (config(), "input.multitap" lname ".right",  "", "none"); \
string_setting Input::uname::a     (config(), "input.multitap" lname ".a",      "", "none"); \
string_setting Input::uname::b     (config(), "input.multitap" lname ".b",      "", "none"); \
string_setting Input::uname::x     (config(), "input.multitap" lname ".x",      "", "none"); \
string_setting Input::uname::y     (config(), "input.multitap" lname ".y",      "", "none"); \
string_setting Input::uname::l     (config(), "input.multitap" lname ".l",      "", "none"); \
string_setting Input::uname::r     (config(), "input.multitap" lname ".r",      "", "none"); \
string_setting Input::uname::select(config(), "input.multitap" lname ".select", "", "none"); \
string_setting Input::uname::start (config(), "input.multitap" lname ".start",  "", "none");

DeclMultitap(Multitap1A, "1a")
DeclMultitap(Multitap1B, "1b")
DeclMultitap(Multitap1C, "1c")
DeclMultitap(Multitap1D, "1d")
DeclMultitap(Multitap2A, "2a")
DeclMultitap(Multitap2B, "2b")
DeclMultitap(Multitap2C, "2c")
DeclMultitap(Multitap2D, "2d")

#undef DeclMultitap

string_setting Input::GUI::load              (config(), "input.gui.load",               "", "none");
string_setting Input::GUI::pause             (config(), "input.gui.pause",              "", "f12");
string_setting Input::GUI::reset             (config(), "input.gui.reset",              "", "none");
string_setting Input::GUI::power             (config(), "input.gui.power",              "", "none");
string_setting Input::GUI::quit              (config(), "input.gui.quit",               "", "none");
string_setting Input::GUI::speed_decrease    (config(), "input.gui.speed_decrease",     "", "divide");
string_setting Input::GUI::speed_increase    (config(), "input.gui.speed_increase",     "", "multiply");
string_setting Input::GUI::frameskip_decrease(config(), "input.gui.frameskip_decrease", "", "subtract");
string_setting Input::GUI::frameskip_increase(config(), "input.gui.frameskip_increase", "", "add");
string_setting Input::GUI::toggle_fullscreen (config(), "input.gui.toggle_fullscreen",  "", "f11");
string_setting Input::GUI::toggle_menubar    (config(), "input.gui.toggle_menubar",     "", "escape");
string_setting Input::GUI::toggle_statusbar  (config(), "input.gui.toggle_statusbar",   "", "escape");

struct Misc {
  static integral_setting opacity;
  static integral_setting status_enable;
} misc;

integral_setting Misc::opacity(config(), "misc.opacity", "Opacity of user interface windows", integral_setting::decimal, 100);
integral_setting Misc::status_enable(config(), "misc.status_enable", "Display information statusbar", integral_setting::boolean, true);

struct Advanced {
  static integral_setting enable;
} advanced;

integral_setting Advanced::enable(config(), "advanced.enable", "Enable advanced, developer-oriented UI options", integral_setting::boolean, false);

} //namespace config
