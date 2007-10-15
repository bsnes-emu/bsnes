namespace config {

char filename[PATH_MAX + 16] = "bsnes.cfg";

struct System {
  static StringSetting video, audio, input;
  static StringSetting video_flags, audio_flags, input_flags;

  static IntegerSetting regulate_speed, speed;
  static IntegerSetting speed_slowest, speed_slow, speed_normal, speed_fast, speed_fastest;
} system;
StringSetting System::video(&config(), "system.video", "Video hardware interface", "");
StringSetting System::audio(&config(), "system.audio", "Audio hardware interface", "");
StringSetting System::input(&config(), "system.input", "Input hardware interface", "");

StringSetting System::video_flags(&config(), "system.video_flags", "Video hardware interface flags", "");
StringSetting System::audio_flags(&config(), "system.audio_flags", "Audio hardware interface flags", "");
StringSetting System::input_flags(&config(), "system.input_flags", "Input hardware interface flags", "");

IntegerSetting System::regulate_speed(&config(), "system.regulate_speed", "Regulate speed to 60hz (NTSC) / 50hz (PAL)", IntegerSetting::Boolean, true);
IntegerSetting System::speed         (0,            "system.speed",          "Current speed regulation setting (1-5)", IntegerSetting::Decimal, 3);
IntegerSetting System::speed_slowest (&config(), "system.speed_slowest",  "Slowest speed setting", IntegerSetting::Decimal,  50);
IntegerSetting System::speed_slow    (&config(), "system.speed_slow",     "Slow speed setting",    IntegerSetting::Decimal,  75);
IntegerSetting System::speed_normal  (&config(), "system.speed_normal",   "Normal speed setting",  IntegerSetting::Decimal, 100);
IntegerSetting System::speed_fast    (&config(), "system.speed_fast",     "Fast speed setting",    IntegerSetting::Decimal, 150);
IntegerSetting System::speed_fastest (&config(), "system.speed_fastest",  "Fastest speed setting", IntegerSetting::Decimal, 200);

struct Video {
  static IntegerSetting mode;
  struct Windowed {
    static IntegerSetting synchronize, aspect_correction;
    static IntegerSetting region, multiplier, hardware_filter, software_filter;
  } windowed;
  struct Fullscreen {
    static IntegerSetting synchronize, aspect_correction;
    static IntegerSetting region, multiplier, hardware_filter, software_filter;
  } fullscreen;
  static IntegerSetting aspect_ntsc_x, aspect_ntsc_y, aspect_pal_x, aspect_pal_y;
  static IntegerSetting frameskip;
  static IntegerSetting use_vram;
} video;

//0 = windowed, 1 = fullscreen, 2 = exclusive
IntegerSetting Video::mode(0, "video.mode", "Active video mode", IntegerSetting::Decimal, 0);

IntegerSetting Video::Windowed::synchronize(&config(), "video.windowed.synchronize", "Synchronize to video refresh rate", IntegerSetting::Boolean, false);
IntegerSetting Video::Windowed::aspect_correction(&config(), "video.windowed.aspect_correction",
  "Correct video aspect ratio\n"
  "Defaults assume display pixels are perfectly square\n"
  "Formula: width = width * video.aspect_<region>_x / video.aspect_<region>_y\n",
  IntegerSetting::Boolean, true);
IntegerSetting Video::Windowed::region(&config(), "video.windowed.region", "Video output region\n"
  "0 = NTSC, 1 = PAL",
  IntegerSetting::Decimal, 0);
IntegerSetting Video::Windowed::multiplier(&config(), "video.windowed.multiplier", "Video output size multiplier (1-5x)\n"
  "1 = 1x (<= 320x240)\n"
  "2 = 2x (<= 640x480)\n"
  "etc.",
  IntegerSetting::Decimal, 2);
IntegerSetting Video::Windowed::hardware_filter(&config(), "video.windowed.hardware_filter", "Video hardware filter\n"
  "0 = Point\n"
  "1 = Linear\n",
  IntegerSetting::Decimal, 1);
IntegerSetting Video::Windowed::software_filter(&config(), "video.windowed.software_filter", "Video software filter\n"
  "0 = None\n"
  "1 = NTSC\n"
  "2 = HQ2x\n"
  "3 = Scale2x\n",
  IntegerSetting::Decimal, 0);

IntegerSetting Video::Fullscreen::synchronize      (&config(), "video.fullscreen.synchronize",       "", IntegerSetting::Boolean, false);
IntegerSetting Video::Fullscreen::aspect_correction(&config(), "video.fullscreen.aspect_correction", "", IntegerSetting::Boolean, true);
IntegerSetting Video::Fullscreen::region           (&config(), "video.fullscreen.region",            "", IntegerSetting::Decimal, 0);
IntegerSetting Video::Fullscreen::multiplier       (&config(), "video.fullscreen.multiplier",        "", IntegerSetting::Decimal, 2);
IntegerSetting Video::Fullscreen::hardware_filter  (&config(), "video.fullscreen.hardware_filter",   "", IntegerSetting::Decimal, 1);
IntegerSetting Video::Fullscreen::software_filter  (&config(), "video.fullscreen.software_filter",   "", IntegerSetting::Decimal, 0);

IntegerSetting Video::aspect_ntsc_x(&config(), "video.aspect_ntsc_x", "", IntegerSetting::Decimal, 54);
IntegerSetting Video::aspect_ntsc_y(&config(), "video.aspect_ntsc_y", "", IntegerSetting::Decimal, 47);
IntegerSetting Video::aspect_pal_x (&config(), "video.aspect_pal_x",  "", IntegerSetting::Decimal, 32);
IntegerSetting Video::aspect_pal_y (&config(), "video.aspect_pal_y",  "", IntegerSetting::Decimal, 23);

IntegerSetting Video::frameskip(0, "video.frameskip", "Video frameskip", IntegerSetting::Decimal, 0);
IntegerSetting Video::use_vram(&config(), "video.use_vram", "Use Video RAM instead of System RAM when possible", IntegerSetting::Boolean, true);

struct Audio {
  static IntegerSetting synchronize;
  static IntegerSetting frequency;
  static IntegerSetting latency;
} audio;
IntegerSetting Audio::synchronize(&config(), "audio.synchronize", "Synchronize to audio sample rate.", IntegerSetting::Boolean, true);
IntegerSetting Audio::frequency(&config(), "audio.frequency", "Default audio playback frequency.", IntegerSetting::Decimal, 32000);
IntegerSetting Audio::latency(&config(), "audio.latency", "Audio playback latency in milliseconds.\n"
  "Specifies how long audio playback is delayed compared to a real SNES.\n"
  "A delay is necessary to allow smooth audio playback via buffering.\n"
  "Raising this value may help with audio playback problems, but will decrease\n"
  "audio responsiveness.",
  IntegerSetting::Decimal, 75);

struct Input {
  static IntegerSetting axis_resistance;
  static IntegerSetting allow_invalid_input;

  struct Joypad1 {
    static StringSetting up, down, left, right, a, b, x, y, l, r, select, start;
  } joypad1;

  struct Joypad2 {
    static StringSetting up, down, left, right, a, b, x, y, l, r, select, start;
  } joypad2;
} input;

IntegerSetting Input::axis_resistance(&config(), "input.axis_resistance",
  "Axis resistance for all analog joypads\n"
  "Affects responsiveness of analog stick movement by specifying what percentage\n"
  "in any given direction the axis must be pressed to trigger a button press.\n"
  "In other words, this determines how hard you have to press the analog stick to\n"
  "simulate pressing e.g. left or right on a digital joypad.\n"
  "Value is a percentage, from 0 (axis will trigger with virtually any axis movement)\n"
  "up to 100 (axis must be pressed fully to given corner).\n"
  "Value affects all four directions of the axis equally.\n"
  "Note: Values below 10 or above 90 are not recommended and may not work at all.",
  IntegerSetting::Decimal, 75);

IntegerSetting Input::allow_invalid_input(&config(), "input.allow_invalid_input",
  "Allow up+down and left+right combinations (not recommended)",
  IntegerSetting::Boolean, false);

StringSetting Input::Joypad1::up    (&config(), "input.joypad1.up",     "", "up");
StringSetting Input::Joypad1::down  (&config(), "input.joypad1.down",   "", "down");
StringSetting Input::Joypad1::left  (&config(), "input.joypad1.left",   "", "left");
StringSetting Input::Joypad1::right (&config(), "input.joypad1.right",  "", "right");
StringSetting Input::Joypad1::a     (&config(), "input.joypad1.a",      "", "x");
StringSetting Input::Joypad1::b     (&config(), "input.joypad1.b",      "", "z");
StringSetting Input::Joypad1::x     (&config(), "input.joypad1.x",      "", "s");
StringSetting Input::Joypad1::y     (&config(), "input.joypad1.y",      "", "a");
StringSetting Input::Joypad1::l     (&config(), "input.joypad1.l",      "", "d");
StringSetting Input::Joypad1::r     (&config(), "input.joypad1.r",      "", "c");
StringSetting Input::Joypad1::select(&config(), "input.joypad1.select", "", "rshift");
StringSetting Input::Joypad1::start (&config(), "input.joypad1.start",  "", "enter");

StringSetting Input::Joypad2::up    (&config(), "input.joypad2.up",     "", "t");
StringSetting Input::Joypad2::down  (&config(), "input.joypad2.down",   "", "g");
StringSetting Input::Joypad2::left  (&config(), "input.joypad2.left",   "", "f");
StringSetting Input::Joypad2::right (&config(), "input.joypad2.right",  "", "h");
StringSetting Input::Joypad2::a     (&config(), "input.joypad2.a",      "", "k");
StringSetting Input::Joypad2::b     (&config(), "input.joypad2.b",      "", "j");
StringSetting Input::Joypad2::x     (&config(), "input.joypad2.x",      "", "i");
StringSetting Input::Joypad2::y     (&config(), "input.joypad2.y",      "", "u");
StringSetting Input::Joypad2::l     (&config(), "input.joypad2.l",      "", "o");
StringSetting Input::Joypad2::r     (&config(), "input.joypad2.r",      "", "l");
StringSetting Input::Joypad2::select(&config(), "input.joypad2.select", "", "lbracket");
StringSetting Input::Joypad2::start (&config(), "input.joypad2.start",  "", "rbracket");

struct Misc {
  static IntegerSetting show_frame_counter;
} misc;

IntegerSetting Misc::show_frame_counter(&config(), "misc.show_frame_counter", "Display frame counter", IntegerSetting::Boolean, true);

};
