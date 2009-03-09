class Configuration : public configuration {
public:
  struct System {
    string video, audio, input;
    bool crashedOnLastRun;
    unsigned speed;
  } system;

  struct Video {
    bool isFullscreen;
    bool synchronize;
    signed contrastAdjust, brightnessAdjust, gammaAdjust;
    bool enableGammaRamp;
    bool enableNtscMergeFields;
    double ntscAspectRatio, palAspectRatio;

    struct Context {
      bool correctAspectRatio;
      unsigned multiplier, region;
      unsigned hwFilter, swFilter;
    } *context, windowed, fullscreen;
  } video;

  struct Audio {
    bool synchronize;
    bool mute;
    unsigned volume, latency, outputFrequency, inputFrequency;
  } audio;

  struct Input {
    enum policy_t { FocusPolicyPauseEmulation, FocusPolicyIgnoreInput, FocusPolicyAllowInput };
    unsigned focusPolicy;
    unsigned analogAxisResistance;
    bool allowInvalidInput;

    struct Joypad {
      string up, down, left, right, a, b, x, y, l, r, select, start;
    } joypad1, joypad2,
      multitap1a, multitap1b, multitap1c, multitap1d,
      multitap2a, multitap2b, multitap2c, multitap2d;

    struct Mouse {
      string x, y, left, right;
    } mouse1, mouse2;

    struct SuperScope {
      string x, y, trigger, turbo, cursor, pause;
    } superscope;

    struct Justifier {
      string x, y, trigger, start;
    } justifier1, justifier2;

    struct UiGeneral {
      string loadCartridge;
      string pauseEmulation;
      string resetSystem;
      string powerCycleSystem;
      string lowerSpeed;
      string raiseSpeed;
      string toggleCheatSystem;
      string toggleFullscreen;
      string toggleMenu;
      string toggleStatus;
      string exitEmulator;
    } uiGeneral;
  } input;

  Configuration() {
    //========
    //external
    //========

    attach(snes.config.controller_port1 = SNES::Input::DeviceJoypad, "snes.controller_port1");
    attach(snes.config.controller_port2 = SNES::Input::DeviceJoypad, "snes.controller_port2");
    attach(snes.config.expansion_port   = SNES::ExpansionBSX,        "snes.expansion_port");
    attach(snes.config.region           = SNES::Autodetect,          "snes.region");

    attach(snes.config.file.autodetect_type    = false, "file.autodetect_type");
    attach(snes.config.file.bypass_patch_crc32 = false, "file.bypass_patch_crc32");

    attach(snes.config.path.rom   = "", "path.rom");
    attach(snes.config.path.save  = "", "path.save");
    attach(snes.config.path.patch = "", "path.patch");
    attach(snes.config.path.cheat = "", "path.cheat");
    attach(snes.config.path.data  = "", "path.data");
    attach(snes.config.path.bsx   = "", "path.bsx");
    attach(snes.config.path.st    = "", "path.st");

    attach(snes.config.cpu.version         =        2, "cpu.version", "Valid version(s) are: 1, 2");
    attach(snes.config.cpu.ntsc_clock_rate = 21477272, "cpu.ntsc_clock_rate");
    attach(snes.config.cpu.pal_clock_rate  = 21281370, "cpu.pal_clock_rate");
    attach(snes.config.cpu.alu_mul_delay   =        2, "cpu.alu_mul_delay");
    attach(snes.config.cpu.alu_div_delay   =        2, "cpu.alu_div_delay");
    attach(snes.config.cpu.wram_init_value =     0x55, "cpu.wram_init_value");

    attach(snes.config.smp.ntsc_clock_rate = 32041 * 768, "smp.ntsc_clock_rate");
    attach(snes.config.smp.pal_clock_rate  = 32041 * 768, "smp.pal_clock_rate");

    attach(snes.config.ppu1.version = 1, "ppu1.version", "Valid version(s) are: 1");
    attach(snes.config.ppu2.version = 3, "ppu2.version", "Valid version(s) are: 1, 2, 3");

    //========
    //internal
    //========

    attach(system.video = "", "system.video");
    attach(system.audio = "", "system.audio");
    attach(system.input = "", "system.input");
    attach(system.crashedOnLastRun = false, "system.crashedOnLastRun");
    attach(system.speed = 2, "system.speed");

    video.context = &video.windowed;
    attach(video.isFullscreen = false, "video.isFullscreen");
    attach(video.synchronize  = false, "video.synchronize");

    attach(video.contrastAdjust   = 0, "video.contrastAdjust");
    attach(video.brightnessAdjust = 0, "video.brightnessAdjust");
    attach(video.gammaAdjust      = 0, "video.gammaAdjust");

    attach(video.enableGammaRamp       =  true, "video.enableGammaRamp");
    attach(video.enableNtscMergeFields = false, "video.enableNtscMergeFields");

    attach(video.ntscAspectRatio = 54.0 / 47.0, "video.ntscAspectRatio", "NTSC aspect ratio (x / y)");
    attach(video.palAspectRatio  = 32.0 / 23.0, "video.palAspectRatio",  "PAL aspect ratio (x / y)");

    attach(video.windowed.correctAspectRatio = true, "video.windowed.correctAspectRatio");
    attach(video.windowed.multiplier         =    2, "video.windowed.multiplier");
    attach(video.windowed.region             =    0, "video.windowed.region");

    attach(video.windowed.hwFilter = 1, "video.windowed.hwFilter");
    attach(video.windowed.swFilter = 0, "video.windowed.swFilter");

    attach(video.fullscreen.correctAspectRatio = true, "video.fullscreen.correctAspectRatio");
    attach(video.fullscreen.multiplier         =    9, "video.fullscreen.multiplier");
    attach(video.fullscreen.region             =    0, "video.fullscreen.region");

    attach(video.fullscreen.hwFilter = 1, "video.fullscreen.hwFilter");
    attach(video.fullscreen.swFilter = 0, "video.fullscreen.swFilter");

    attach(audio.synchronize = true,  "audio.synchronize");
    attach(audio.mute        = false, "audio.mute");

    attach(audio.volume          =   100, "audio.volume");
    attach(audio.latency         =    80, "audio.latency");
    attach(audio.outputFrequency = 48000, "audio.outputFrequency");
    attach(audio.inputFrequency  = 32000, "audio.inputFrequency");

    attach(input.focusPolicy = Input::FocusPolicyPauseEmulation, "input.focusPolicy");
    attach(input.analogAxisResistance =    50, "input.analogAxisResistance", "Percentage; lower = less resistance");
    attach(input.allowInvalidInput    = false, "input.allowInvalidInput", "Allow up+down / left+right combinations; may trigger bugs in some games");

    attach(input.joypad1.up     = "up",     "input.joypad1.up");
    attach(input.joypad1.down   = "down",   "input.joypad1.down");
    attach(input.joypad1.left   = "left",   "input.joypad1.left");
    attach(input.joypad1.right  = "right",  "input.joypad1.right");
    attach(input.joypad1.a      = "x",      "input.joypad1.a");
    attach(input.joypad1.b      = "z",      "input.joypad1.b");
    attach(input.joypad1.x      = "s",      "input.joypad1.x");
    attach(input.joypad1.y      = "a",      "input.joypad1.y");
    attach(input.joypad1.l      = "d",      "input.joypad1.l");
    attach(input.joypad1.r      = "c",      "input.joypad1.r");
    attach(input.joypad1.select = "rshift", "input.joypad1.select");
    attach(input.joypad1.start  = "return", "input.joypad1.start");

    attachJoypad(input.joypad2,    "input.joypad2");
    attachJoypad(input.multitap1a, "input.multitap1a");
    attachJoypad(input.multitap1b, "input.multitap1b");
    attachJoypad(input.multitap1c, "input.multitap1c");
    attachJoypad(input.multitap1d, "input.multitap1d");
    attachJoypad(input.multitap2a, "input.multitap2a");
    attachJoypad(input.multitap2b, "input.multitap2b");
    attachJoypad(input.multitap2c, "input.multitap2c");
    attachJoypad(input.multitap2d, "input.multitap2d");

    attach(input.mouse1.x     = "mouse.x",        "input.mouse1.x");
    attach(input.mouse1.y     = "mouse.y",        "input.mouse1.y");
    attach(input.mouse1.left  = "mouse.button00", "input.mouse1.left");
    attach(input.mouse1.right = "mouse.button02", "input.mouse1.right");

    attach(input.mouse2.x     = "mouse.x",        "input.mouse2.x");
    attach(input.mouse2.y     = "mouse.y",        "input.mouse2.y");
    attach(input.mouse2.left  = "mouse.button00", "input.mouse2.left");
    attach(input.mouse2.right = "mouse.button02", "input.mouse2.right");

    attach(input.superscope.x       = "mouse.x",        "input.superscope.x");
    attach(input.superscope.y       = "mouse.y",        "input.superscope.y");
    attach(input.superscope.trigger = "mouse.button00", "input.superscope.trigger");
    attach(input.superscope.cursor  = "mouse.button02", "input.superscope.cursor");
    attach(input.superscope.turbo   = "t",              "input.superscope.turbo");
    attach(input.superscope.pause   = "p",              "input.superscope.pause");

    attach(input.justifier1.x       = "mouse.x",        "input.justifier1.x");
    attach(input.justifier1.y       = "mouse.y",        "input.justifier1.y");
    attach(input.justifier1.trigger = "mouse.button00", "input.justifier1.trigger");
    attach(input.justifier1.start   = "mouse.button02", "input.jusitifer1.start");

    attach(input.justifier2.x       = "none", "input.justifier2.x");
    attach(input.justifier2.y       = "none", "input.justifier2.y");
    attach(input.justifier2.trigger = "none", "input.justifier2.trigger");
    attach(input.justifier2.start   = "none", "input.justifier2.start");

    attach(input.uiGeneral.loadCartridge     = "none",     "input.uiGeneral.loadCartridge");
    attach(input.uiGeneral.pauseEmulation    = "pause",    "input.uiGeneral.pauseEmulation");
    attach(input.uiGeneral.resetSystem       = "none",     "input.uiGeneral.resetSystem");
    attach(input.uiGeneral.powerCycleSystem  = "none",     "input.uiGeneral.powerCycleSystem");
    attach(input.uiGeneral.lowerSpeed        = "subtract", "input.uiGeneral.lowerSpeed");
    attach(input.uiGeneral.raiseSpeed        = "add",      "input.uiGeneral.raiseSpeed");
    attach(input.uiGeneral.toggleCheatSystem = "none",     "input.uiGeneral.toggleCheatSystem");
    attach(input.uiGeneral.toggleFullscreen  = "f11",      "input.uiGeneral.toggleFullscreen");
    attach(input.uiGeneral.toggleMenu        = "escape",   "input.uiGeneral.toggleMenu");
    attach(input.uiGeneral.toggleStatus      = "escape",   "input.uiGeneral.toggleStatus");
    attach(input.uiGeneral.exitEmulator      = "none",     "input.uiGeneral.exitEmulator");
  }

  void attachJoypad(Input::Joypad &joypad, const char *name) {
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

  bool load(const char *filename) {
    if(configuration::load(filename) == false) return false;
    video.context = (video.isFullscreen == false) ? &video.windowed : &video.fullscreen;
    return true;
  }
} config;
