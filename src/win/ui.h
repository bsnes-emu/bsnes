enum {
  VIDEOMODE_256x224w = 0,
  VIDEOMODE_512x448w,
  VIDEOMODE_960x720w,
  VIDEOMODE_640x480f,
  VIDEOMODE_1024x768f
};

enum {
  MENU_FILE_LOAD = 100,
  MENU_FILE_UNLOAD,
  MENU_FILE_RESET,
  MENU_FILE_POWER,
  MENU_FILE_EXIT,
  MENU_SETTINGS_FRAMESKIP_OFF,
  MENU_SETTINGS_FRAMESKIP_1,
  MENU_SETTINGS_FRAMESKIP_2,
  MENU_SETTINGS_FRAMESKIP_3,
  MENU_SETTINGS_FRAMESKIP_4,
  MENU_SETTINGS_FRAMESKIP_5,
  MENU_SETTINGS_FRAMESKIP_6,
  MENU_SETTINGS_FRAMESKIP_7,
  MENU_SETTINGS_FRAMESKIP_8,
  MENU_SETTINGS_FRAMESKIP_9,
  MENU_SETTINGS_VIDEOMODE_256x224w,
  MENU_SETTINGS_VIDEOMODE_512x448w,
  MENU_SETTINGS_VIDEOMODE_960x720w,
  MENU_SETTINGS_VIDEOMODE_640x480f,
  MENU_SETTINGS_VIDEOMODE_1024x768f,
  MENU_SETTINGS_USEVRAM,
  MENU_SETTINGS_VBLANK,
  MENU_SETTINGS_COLORCURVE,
  MENU_SETTINGS_SHOWFPS,
  MENU_SETTINGS_DEBUGGER
};

enum {
  CONSOLE_OUTPUT = 100,
  CONSOLE_STATUS,

  CONSOLE_CPUGROUP,
  CONSOLE_CPUSTEP,
  CONSOLE_CPUPROCEED,
  CONSOLE_CPUSKIP,
  CONSOLE_CPUTRACENUM,
  CONSOLE_CPUTRACE,
  CONSOLE_CPUDISABLE,

  CONSOLE_APUGROUP,
  CONSOLE_APUSTEP,
  CONSOLE_APUPROCEED,
  CONSOLE_APUSKIP,
  CONSOLE_APUTRACENUM,
  CONSOLE_APUTRACE,
  CONSOLE_APUDISABLE,

  CONSOLE_SYSGROUP,
  CONSOLE_SYSRUN,
  CONSOLE_SYSRUNTOFRAME,
  CONSOLE_SYSRUNTONMI,

  CONSOLE_CFGGROUP,
  CONSOLE_CFGOUTPUTCPU,
  CONSOLE_CFGOUTPUTAPU,
  CONSOLE_CFGOUTPUTDBG,
  CONSOLE_CFGTRACE,
  CONSOLE_CFGREGTYPE,
  CONSOLE_CFGREGNUM,
  CONSOLE_CFGREGVAL,
  CONSOLE_CFGREGSET,
  CONSOLE_CFGLOCK,
  CONSOLE_CFGLOCKUP,
  CONSOLE_CFGLOCKDOWN,
  CONSOLE_CFGLOCKLEFT,
  CONSOLE_CFGLOCKRIGHT,
  CONSOLE_CFGLOCKA,
  CONSOLE_CFGLOCKB,
  CONSOLE_CFGLOCKX,
  CONSOLE_CFGLOCKY,
  CONSOLE_CFGLOCKL,
  CONSOLE_CFGLOCKR,
  CONSOLE_CFGLOCKSELECT,
  CONSOLE_CFGLOCKSTART
};

enum {
  BREAKPOINT_LIST = 100,
  BREAKPOINT_STATIC1,
  BREAKPOINT_NUM,
  BREAKPOINT_OFFSET,
  BREAKPOINT_R,
  BREAKPOINT_W,
  BREAKPOINT_X,
  BREAKPOINT_V,
  BREAKPOINT_STATIC2,
  BREAKPOINT_VALUE,
  BREAKPOINT_SOURCE,
  BREAKPOINT_SET,
  BREAKPOINT_CLEAR,
  BREAKPOINT_ENABLE,
  BREAKPOINT_CLEARALL,
  BREAKPOINT_EXPORT,
  BREAKPOINT_IMPORT
};

enum {
  MEMORYEDITOR_VIEW = 100,

  MEMORYEDITOR_MODE,
  MEMORYEDITOR_GOTOADDR,
  MEMORYEDITOR_GOTO,

  MEMORYEDITOR_STATIC1,
  MEMORYEDITOR_OFFSET,
  MEMORYEDITOR_VALUE,
  MEMORYEDITOR_EDIT,

  MEMORYEDITOR_FSOURCE,
  MEMORYEDITOR_FEXPORT,

  MEMORYEDITOR_UP40,
  MEMORYEDITOR_DOWN40,
  MEMORYEDITOR_UP400,
  MEMORYEDITOR_DOWN400,
  MEMORYEDITOR_UP4000,
  MEMORYEDITOR_DOWN4000,
  MEMORYEDITOR_UP40000,
  MEMORYEDITOR_DOWN40000
};

class Window {
public:
HWND  hwnd;
HMENU hmenu;
bool fullscreen, visible, cursor_visible;
RECT workarea, wa_offset;
struct { int width, height; }window;
  void resize(int width, int height);
  void center();
  void show_menu();
  void hide_menu();
  void show();
  void hide();

  void to_left(HWND _hwnd = 0);
  void to_center();
  void to_right();
  void to_top();
  void to_middle();
  void to_bottom(HWND _hwnd = 0);

  Window();
};

class MainWindow : public Window {
public:
uint8 frameskip;
int width, height;
  void to_fullscreen();
  void to_windowed();
  void set_frameskip(uint8 fs);
  void adjust_video_mode(bool fullscreen_mode);
  void set_video_mode(uint8 mode);
  void menu_load();
  void menu_unload();

  MainWindow();
};

MainWindow *w_main;

#define CONSOLE_LINES 256
class Console : public Window {
private:
bool _is_running; //used to swap "Run"/"Stop" text on console window
public:
bool ctl_disabled[100];

enum { DEBUG_MESSAGE = 0, CPU_MESSAGE, APU_MESSAGE };
enum { REGTYPE_CPU = 0, REGTYPE_APU };
char output_line[CONSOLE_LINES][512];
char output_data[CONSOLE_LINES * 512];
FILE *log_fp;
bool tracing_enabled;

struct {
  bool up, down, left, right;
  bool a, b, x, y;
  bool l, r, select, start;
}joypad_lock;

  void clear();
  void create_controls();
  void update_status();
  void set_reg_list_type(uint8 list_type);
  bool can_write(uint32 message_type);
  void write(char *s, uint32 message_type = DEBUG_MESSAGE);
  void is_running(bool n); //API access to _is_running

  Console();
};

Console *w_console;

class BreakpointEditor : public Window {
public:
bool ctl_disabled[100];

enum { DRAM = 0, VRAM = 1, OAM = 2, CGRAM = 3 };
enum { FLAG_NONE = 0, FLAG_R = 1, FLAG_W = 2, FLAG_X = 4, FLAG_V = 8 };
struct {
  uint32 addr;
  uint8  value;
  uint8  source;
  uint8  flags;
  uint32 hit_count;
  bool   set;
}bp[16];
bool bp_enabled, bp_hit;
  void clear();
  void create_controls();
  bool hit();
  void test(uint32 message, uint32 addr, uint32 value);
  void refresh();

  BreakpointEditor();
};

BreakpointEditor *w_bp;

class MemoryEditor : public Window {
public:
bool ctl_disabled[100];

enum { MODE_DRAM = 0, MODE_ROM, MODE_SRAM, MODE_VRAM, MODE_OAM, MODE_CGRAM };
uint32 edit_mode, edit_addr, edit_mask;
  void  clear();
  void  create_controls();
  uint8 read_byte(uint32 addr);
  void  refresh(uint32 type = 0, uint32 addr = 0);
  void  export(uint32 type);
};

MemoryEditor *w_memory;
