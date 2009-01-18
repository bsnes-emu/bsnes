#include <nall/config.hpp>

class bsnes_configuration : public configuration {
public:
  char bsnes_cfg[PATH_MAX], locale_cfg[PATH_MAX];

  struct System {
    string video, audio, input;
    bool invoke_crash_handler;
    unsigned emulation_speed;
  } system;

  struct Video {
    unsigned mode;
    bool synchronize;

    struct Windowed {
      bool aspect_correction;
      unsigned region, multiplier, hardware_filter, software_filter;
    } windowed;

    struct Fullscreen {
      bool aspect_correction;
      unsigned region, multiplier, hardware_filter, software_filter;
    } fullscreen;

    unsigned aspect_ntsc_x, aspect_ntsc_y, aspect_pal_x, aspect_pal_y;
    unsigned frameskip;

    unsigned contrast, brightness, gamma;
    bool gamma_ramp, sepia, grayscale, invert;
    bool ntsc_filter_merge_fields;
  } video;

  struct Audio {
    unsigned output_frequency, input_frequency;
    unsigned latency;
    unsigned volume, mute;
    bool synchronize;
  } audio;

  struct Input {
    unsigned capture_mode;
    bool allow_invalid_input;
    unsigned analog_axis_resistance;

    struct Joypad {
      string up, down, left, right, a, b, x, y, l, r, select, start;
    } joypad1, joypad2,
      multitap1a, multitap1b, multitap1c, multitap1d,
      multitap2a, multitap2b, multitap2c, multitap2d;

    struct Mouse {
      string x, y, l, r;
    } mouse1, mouse2;

    struct SuperScope {
      string x, y, trigger, turbo, cursor, pause;
    } superscope;

    struct Justifier {
      string x, y, trigger, start;
    } justifier1, justifier2;

    struct GUI {
      string load, pause, reset, power, quit;
      string speed_decrease, speed_increase;
      string frameskip_decrease, frameskip_increase;
      string toggle_fullscreen, toggle_menubar, toggle_statusbar;
    } gui;

    struct Debugger {
      string export_memory;
      string toggle_cputrace, toggle_cputracemask;
      string toggle_smptrace, toggle_smptracemask;
    } debugger;
  } input;

  struct Misc {
    bool start_in_fullscreen_mode;
    unsigned window_opacity;
    unsigned cheat_autosort;
    bool show_advanced_options;
  } misc;

  bsnes_configuration() {
    //========
    //external
    //========

    attach(snes.config.controller_port1 = SNES::Input::DeviceJoypad, "snes.controller_port1");
    attach(snes.config.controller_port2 = SNES::Input::DeviceJoypad, "snes.controller_port2");
    attach(snes.config.expansion_port   =        SNES::ExpansionBSX, "snes.expansion_port");
    attach(snes.config.region           =          SNES::Autodetect, "snes.region");

    attach(snes.config.file.autodetect_type    = false, "file.autodetect_type", "Detect filetype by header, rather than file extension");
    attach(snes.config.file.bypass_patch_crc32 = false, "file.bypass_patch_crc32", "Apply UPS patches even when checksum match fails");

    attach(snes.config.path.rom        = "", "path.rom");
    attach(snes.config.path.save       = "", "path.save");
    attach(snes.config.path.patch      = "", "path.patch");
    attach(snes.config.path.cheat      = "", "path.cheat");
    attach(snes.config.path.exportdata = "", "path.exportdata");
    attach(snes.config.path.bsx        = "", "path.bsx");
    attach(snes.config.path.st         = "", "path.st");

    attach(snes.config.cpu.ntsc_clock_rate = 21477272, "cpu.ntsc_clock_rate");
    attach(snes.config.cpu.pal_clock_rate  = 21281370, "cpu.pal_clock_rate");
    attach(snes.config.cpu.alu_mul_delay   =        2, "cpu.alu_mul_delay");
    attach(snes.config.cpu.alu_div_delay   =        2, "cpu.alu_div_delay");
    attach(snes.config.cpu.wram_init_value =     0x55, "cpu.wram_init_value");

    attach(snes.config.smp.ntsc_clock_rate = 32041 * 768, "smp.ntsc_clock_rate");
    attach(snes.config.smp.pal_clock_rate  = 32041 * 768, "smp.pal_clock_rate");

    //========
    //internal
    //========

    *bsnes_cfg  = 0;
    *locale_cfg = 0;

    attach(system.video = "", "system.video", "Video hardware interface");
    attach(system.audio = "", "system.audio", "Audio hardware interface");
    attach(system.input = "", "system.input", "Input hardware interface");

    attach(system.invoke_crash_handler = false, "system.invoke_crash_handler", "Do not modify! Used to detect crashes caused by driver initialization");
    attach(system.emulation_speed      =     2, "system.emulation_speed", "Relative speed of emulator compared to hardware");

    video.mode = 0;
    attach(video.synchronize = false, "video.synchronize", "Synchronize to video refresh rate");

    attach(video.windowed.aspect_correction = true, "video.windowed.aspect_correction");
    attach(video.windowed.region            =    0, "video.windowed.region");
    attach(video.windowed.multiplier        =    2, "video.windowed.multiplier");
    attach(video.windowed.hardware_filter   =    1, "video.windowed.hardware_filter");
    attach(video.windowed.software_filter   =    0, "video.windowed.software_filter");

    attach(video.fullscreen.aspect_correction = true, "video.fullscreen.aspect_correction");
    attach(video.fullscreen.region            =    0, "video.fullscreen.region");
    attach(video.fullscreen.multiplier        =    2, "video.fullscreen.multiplier");
    attach(video.fullscreen.hardware_filter   =    1, "video.fullscreen.hardware_filter");
    attach(video.fullscreen.software_filter   =    0, "video.fullscreen.software_filter");

    attach(video.aspect_ntsc_x = 54, "video.aspect_ntsc_x", "NTSC TV aspect correction ratio");
    attach(video.aspect_ntsc_y = 47, "video.aspect_ntsc_y");
    attach(video.aspect_pal_x  = 32, "video.aspect_pal_x", "PAL TV aspect correction ratio");
    attach(video.aspect_pal_y  = 23, "video.aspect_pal_y");

    video.frameskip = 0;
    attach(video.contrast   =   0, "video.contrast");
    attach(video.brightness =   0, "video.brightness");
    attach(video.gamma      = 100, "video.gamma");

    attach(video.gamma_ramp               =  true, "video.gamma_ramp");
    attach(video.sepia                    = false, "video.sepia");
    attach(video.grayscale                = false, "video.grayscale");
    attach(video.invert                   = false, "video.invert");
    attach(video.ntsc_filter_merge_fields =  true, "video.ntsc_filter_merge_fields");

    attach(audio.output_frequency = 48000, "audio.output_frequency");
    attach(audio.input_frequency  = 32000, "audio.input_frequency");
    attach(audio.latency          =   100, "audio.latency");
    attach(audio.volume           =   100, "audio.volume");
    attach(audio.mute             = false, "audio.mute");
    attach(audio.synchronize      =  true, "audio.synchronize");

    attach(input.capture_mode           =     2, "input.capture_mode", "Capture method: 0 = allow, 1 = ignore, 2 = pause");
    attach(input.allow_invalid_input    = false, "input.allow_invalid_input", "Allow D-pad up+down and left+right combinations (not recommended)");
    attach(input.analog_axis_resistance =    50, "input.analog_axis_resistance", "Analog stick resistance percentage; lower = less, higher = more");

    attach(input.joypad1.up     =     "up", "input.joypad1.up");
    attach(input.joypad1.down   =   "down", "input.joypad1.down");
    attach(input.joypad1.left   =   "left", "input.joypad1.left");
    attach(input.joypad1.right  =  "right", "input.joypad1.right");
    attach(input.joypad1.a      =      "x", "input.joypad1.a");
    attach(input.joypad1.b      =      "z", "input.joypad1.b");
    attach(input.joypad1.x      =      "s", "input.joypad1.x");
    attach(input.joypad1.y      =      "a", "input.joypad1.y");
    attach(input.joypad1.l      =      "d", "input.joypad1.l");
    attach(input.joypad1.r      =      "c", "input.joypad1.r");
    attach(input.joypad1.select = "rshift", "input.joypad1.select");
    attach(input.joypad1.start  = "return", "input.joypad1.start");

    attach_joypad(input.joypad2,    "input.joypad2");
    attach_joypad(input.multitap1a, "input.multitap1a");
    attach_joypad(input.multitap1b, "input.multitap1b");
    attach_joypad(input.multitap1c, "input.multitap1c");
    attach_joypad(input.multitap1d, "input.multitap1d");
    attach_joypad(input.multitap2a, "input.multitap2a");
    attach_joypad(input.multitap2b, "input.multitap2b");
    attach_joypad(input.multitap2c, "input.multitap2c");
    attach_joypad(input.multitap2d, "input.multitap2d");

    attach(input.mouse1.x =        "mouse1.x", "input.mouse1.x");
    attach(input.mouse1.y =        "mouse1.y", "input.mouse1.y");
    attach(input.mouse1.l = "mouse1.button00", "input.mouse1.l");
    attach(input.mouse1.r = "mouse1.button02", "input.mouse1.r");

    attach(input.mouse2.x =        "mouse2.x", "input.mouse2.x");
    attach(input.mouse2.y =        "mouse2.y", "input.mouse2.y");
    attach(input.mouse2.l = "mouse2.button00", "input.mouse2.l");
    attach(input.mouse2.r = "mouse2.button02", "input.mouse2.r");

    attach(input.superscope.x       =        "mouse.x", "input.superscope.x");
    attach(input.superscope.y       =        "mouse.y", "input.superscope.y");
    attach(input.superscope.trigger = "mouse.button00", "input.superscope.trigger");
    attach(input.superscope.cursor  = "mouse.button02", "input.superscope.cursor");
    attach(input.superscope.turbo   =              "t", "input.superscope.turbo");
    attach(input.superscope.pause   =              "p", "input.superscope.pause");

    attach(input.justifier1.x       =        "mouse.x", "input.justifier1.x");
    attach(input.justifier1.y       =        "mouse.y", "input.justifier1.y");
    attach(input.justifier1.trigger = "mouse.button00", "input.justifier1.trigger");
    attach(input.justifier1.start   = "mouse.button02", "input.justifier1.start");

    attach(input.justifier2.x       = "none", "input.justifier2.x");
    attach(input.justifier2.y       = "none", "input.justifier2.y");
    attach(input.justifier2.trigger = "none", "input.justifier2.trigger");
    attach(input.justifier2.start   = "none", "input.justifier2.start");

    attach(input.gui.load  = "none", "input.gui.load");
    attach(input.gui.pause =  "f12", "input.gui.pause");
    attach(input.gui.reset = "none", "input.gui.reset");
    attach(input.gui.power = "none", "input.gui.power");
    attach(input.gui.quit  = "none", "input.gui.quit");

    attach(input.gui.speed_decrease     =   "divide", "input.gui.speed_decrease");
    attach(input.gui.speed_increase     = "multiply", "input.gui.speed_increase");
    attach(input.gui.frameskip_decrease = "subtract", "input.gui.frameskip_decrease");
    attach(input.gui.frameskip_increase =      "add", "input.gui.frameskip_increase");

    attach(input.gui.toggle_fullscreen =    "f11", "input.gui.toggle_fullscreen");
    attach(input.gui.toggle_menubar    = "escape", "input.gui.toggle_menubar");
    attach(input.gui.toggle_statusbar  = "escape", "input.gui.toggle_statusbar");

    attach(input.debugger.export_memory       = "none", "input.debugger.export_memory");
    attach(input.debugger.toggle_cputrace     = "none", "input.debugger.toggle_cputrace");
    attach(input.debugger.toggle_cputracemask = "none", "input.debugger.toggle_cputracemask");
    attach(input.debugger.toggle_smptrace     = "none", "input.debugger.toggle_smptrace");
    attach(input.debugger.toggle_smptracemask = "none", "input.debugger.toggle_smptracemask");

    attach(misc.start_in_fullscreen_mode = false, "misc.start_in_fullscreen_mode");
    attach(misc.window_opacity           =   100, "misc.window_opacity", "Translucency percentage of helper windows (50%-100%)");
    attach(misc.cheat_autosort           = false, "misc.cheat_autosort");
    attach(misc.show_advanced_options    = false, "misc.show_advanced_options", "Enable developer-oriented GUI options");
  }

  void attach_joypad(Input::Joypad &joypad, const char *name) {
    attach(joypad.up     = "none", string() << name << ".up");
    attach(joypad.down   = "none", string() << name << ".down");
    attach(joypad.left   = "none", string() << name << ".left");
    attach(joypad.right  = "none", string() << name << ".right");
    attach(joypad.a      = "none", string() << name << ".a");
    attach(joypad.b      = "none", string() << name << ".b");
    attach(joypad.x      = "none", string() << name << ".x");
    attach(joypad.y      = "none", string() << name << ".y");
    attach(joypad.l      = "none", string() << name << ".l");
    attach(joypad.r      = "none", string() << name << ".r");
    attach(joypad.select = "none", string() << name << ".select");
    attach(joypad.start  = "none", string() << name << ".start");
  }
} config;
