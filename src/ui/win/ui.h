#define KeyDown(__key) ((GetAsyncKeyState(__key) & 0x8000) ? 1 : 0)

enum {
  EVENT_INPUTKEYDOWN = EVENT_USER + 0,
  EVENT_INPUTKEYUP   = EVENT_USER + 1,
};

namespace global {
Font vwf, fwf, font_about, font_header, font_list;
};

enum {
MENU_FILE = 100,
  MENU_FILE_LOAD,
  MENU_FILE_UNLOAD,
  MENU_FILE_RESET,
  MENU_FILE_POWER,
  MENU_FILE_EXIT,

MENU_SETTINGS,
  MENU_SETTINGS_VIDEOPROFILE,
    MENU_SETTINGS_VIDEOPROFILE_0,
    MENU_SETTINGS_VIDEOPROFILE_1,
    MENU_SETTINGS_VIDEOPROFILE_2,
    MENU_SETTINGS_VIDEOPROFILE_3,
    MENU_SETTINGS_VIDEOPROFILE_4,
    MENU_SETTINGS_VIDEOPROFILE_5,
    MENU_SETTINGS_VIDEOPROFILE_6,
    MENU_SETTINGS_VIDEOPROFILE_7,
  MENU_SETTINGS_FRAMESKIP,
    MENU_SETTINGS_FRAMESKIP_0,
  //---
    MENU_SETTINGS_FRAMESKIP_1,
    MENU_SETTINGS_FRAMESKIP_2,
    MENU_SETTINGS_FRAMESKIP_3,
    MENU_SETTINGS_FRAMESKIP_4,
    MENU_SETTINGS_FRAMESKIP_5,
    MENU_SETTINGS_FRAMESKIP_6,
    MENU_SETTINGS_FRAMESKIP_7,
    MENU_SETTINGS_FRAMESKIP_8,
    MENU_SETTINGS_FRAMESKIP_9,
  MENU_SETTINGS_SHOWFPS,
//---
  MENU_SETTINGS_MUTE,
//---
  MENU_SETTINGS_SPEED_REGULATION,
    MENU_SETTINGS_SPEED_REGULATION_ENABLE,
  //---
    MENU_SETTINGS_SPEED_REGULATION_SLOWEST,
    MENU_SETTINGS_SPEED_REGULATION_SLOW,
    MENU_SETTINGS_SPEED_REGULATION_NORMAL,
    MENU_SETTINGS_SPEED_REGULATION_FAST,
    MENU_SETTINGS_SPEED_REGULATION_FASTEST,
  MENU_SETTINGS_CONFIGURATION,
  MENU_SETTINGS_DEBUGGER,

MENU_MISC,
  MENU_MISC_SCREENSHOT,
  MENU_MISC_LOGAUDIO,
  MENU_MISC_CHEATEDITOR,
//---
  MENU_MISC_ABOUT,
};

class MainWindow : public Window {
public:
uint8 frameskip, frameskip_pos;
uint8 regulation_speed;

struct {
  bool   fullscreen;
  uint32 x, y, refresh_rate;
  uint32 width, height;
} vi;
  void SetFrameskip(uint fs);
  void SetRegulationSpeed(uint speed);

  bool Event(EventInfo &info);

  void Setup();

  MainWindow() { frameskip = frameskip_pos = 0; }
} wMain;

class AboutWindow : public Window {
public:
Editbox AboutText;
Button  Ok;

static const char about_text[4096];
  bool Event(EventInfo &info);

  void Setup();
} wAbout;

#include "settings/settings.h"
#include "debugger/debugger.h"
