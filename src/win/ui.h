#define KeyDown(__key) ((GetAsyncKeyState(__key) & 0x8000) ? 1 : 0)

#include "video/video.h"
#include "audio/audio.h"
#include "input/input.h"
Video *uiVideo;
Audio *uiAudio;
Input *uiInput;

namespace global {
HFONT  vwf, fwf;
HBRUSH black_brush, window_brush;
bool   cursor_visible = true;
};

long __stdcall window_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class Window {
public:
HWND  hwnd;
HMENU menu;

enum {
  IDTYPE_UNKNOWN,
  IDTYPE_MENU,
  IDTYPE_BUTTON
};

enum {
  WF_NONE    = 0,
  WF_CENTER  = 1,
  WF_TOPMOST = 2,
};

struct {
  bool  visible;
  bool  fullscreen;
  bool  center;
  int   width, height;
} settings;

  virtual void   hide();
  virtual void   show();
  virtual bool   visible();
  virtual void   hide_menu();
  virtual void   show_menu();
  virtual uint32 get_id_type(uint32 id);
  virtual void   check(uint32 id, bool state);
  virtual bool   checked(uint32 id);
  virtual void   check_toggle(uint32 id);
  virtual void   check(uint32 id);
  virtual void   uncheck(uint32 id);
  virtual void   resize(int width, int height, bool fullscreen = false);
  virtual void   center();

  virtual void   set_text(uint32 id, const char *text, ...);
  virtual void   control(const char *classname, const char *text, uint32 flags, int x, int y, int width, int height, uint32 id);
  virtual long   wndproc(UINT msg, WPARAM wparam, LPARAM lparam);
  virtual void   init(const char *classname, const char *title, int width, int height, HBRUSH hbr, uint32 flags = WF_NONE);

  Window();
};

enum {
MENU_FILE = 100,
  MENU_FILE_LOAD,
  MENU_FILE_UNLOAD,
  MENU_FILE_RESET,
  MENU_FILE_POWER,
  MENU_FILE_EXIT,

MENU_SETTINGS,
  MENU_SETTINGS_VIDEOMODE,
    MENU_SETTINGS_VIDEOMODE_0,
    MENU_SETTINGS_VIDEOMODE_1,
    MENU_SETTINGS_VIDEOMODE_2,
    MENU_SETTINGS_VIDEOMODE_3,
    MENU_SETTINGS_VIDEOMODE_4,
  //---
    MENU_SETTINGS_VIDEOMODE_5,
    MENU_SETTINGS_VIDEOMODE_6,
    MENU_SETTINGS_VIDEOMODE_7,
    MENU_SETTINGS_VIDEOMODE_8,
    MENU_SETTINGS_VIDEOMODE_9,
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
  MENU_SETTINGS_COLORADJUST,
    MENU_SETTINGS_COLORADJUST_COLORCURVE,
    MENU_SETTINGS_COLORADJUST_NORMAL,
    MENU_SETTINGS_COLORADJUST_GRAYSCALE,
    MENU_SETTINGS_COLORADJUST_VGA,
    MENU_SETTINGS_COLORADJUST_GENESIS,
  MENU_SETTINGS_VIDEO_OPTIONS,
    MENU_SETTINGS_VIDEO_OPTIONS_USEVRAM,
    MENU_SETTINGS_VIDEO_OPTIONS_TRIPLEBUF,
    MENU_SETTINGS_VIDEO_OPTIONS_SHOWFPS,
  //---
    MENU_SETTINGS_VIDEO_OPTIONS_PPUCFG,
//---
  MENU_SETTINGS_MUTE,
//---
  MENU_SETTINGS_INPUTCFG,
    MENU_SETTINGS_INPUTCFG_JOYPAD1,
    MENU_SETTINGS_INPUTCFG_JOYPAD2,
//---
  MENU_SETTINGS_SPEED_REGULATION,
    MENU_SETTINGS_SPEED_REGULATION_ENABLE,
  //---
    MENU_SETTINGS_SPEED_REGULATION_SLOWEST,
    MENU_SETTINGS_SPEED_REGULATION_SLOW,
    MENU_SETTINGS_SPEED_REGULATION_NORMAL,
    MENU_SETTINGS_SPEED_REGULATION_FAST,
    MENU_SETTINGS_SPEED_REGULATION_FASTEST,

MENU_MISC,
  MENU_MISC_SCREENSHOT,
  MENU_MISC_LOGAUDIO,
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
  void get_video_mode_info(uint8 mode);
  void set_video_mode(uint8 mode);
  void set_frameskip(uint8 fs);
  void set_regulation_speed(uint8 speed);
  void menu_load();
  void menu_unload();

  long wndproc(UINT msg, WPARAM wparam, LPARAM lparam);
  void setup();

  MainWindow() { frameskip = frameskip_pos = 0; }
} wMain;

enum {
  INPUTCFG_TEXT = 100,
  INPUTCFG_UP,
  INPUTCFG_DOWN,
  INPUTCFG_LEFT,
  INPUTCFG_RIGHT,
  INPUTCFG_A,
  INPUTCFG_B,
  INPUTCFG_X,
  INPUTCFG_Y,
  INPUTCFG_L,
  INPUTCFG_R,
  INPUTCFG_SELECT,
  INPUTCFG_START,
  INPUTCFG_ALLOW_INVALID_INPUT,
};

class InputCfgWindow : public Window {
public:
bool   update_active;
uint32 key_to_update, dev_to_update;
  void timer_key_update();
  void begin_key_update(uint32 key);
  void end_key_update();

  void show();
  void hide();
  long wndproc(UINT msg, WPARAM wparam, LPARAM lparam);
  void setup();
} wInputCfg;

enum {
  PPUCFG_STATIC0,
  PPUCFG_BG1_PRI0,
  PPUCFG_BG1_PRI1,
  PPUCFG_BG2_PRI0,
  PPUCFG_BG2_PRI1,
  PPUCFG_BG3_PRI0,
  PPUCFG_BG3_PRI1,
  PPUCFG_BG4_PRI0,
  PPUCFG_BG4_PRI1,
  PPUCFG_OAM_PRI0,
  PPUCFG_OAM_PRI1,
  PPUCFG_OAM_PRI2,
  PPUCFG_OAM_PRI3,

  PPUCFG_STATIC1,
  PPUCFG_HDMA_ENABLE,
  PPUCFG_OPT_ENABLE,
};

class PPUCfgWindow : public Window {
public:
  long wndproc(UINT msg, WPARAM wparam, LPARAM lparam);
  void setup();
} wPPUCfg;
