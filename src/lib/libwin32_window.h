enum {
  EVENT_CLOSE,
  EVENT_DESTROY,
  EVENT_DRAW,

  EVENT_CLICKED,
  EVENT_DOUBLECLICKED,
  EVENT_MENUCLICKED,
  EVENT_MENUENTER,
  EVENT_MENUEXIT,
  EVENT_CHANGED,

  EVENT_USER = 0x80000000,
};

struct EventInfo {
  uint     event_id;
  uint     event_type;
  Window  *window;
  uint     window_id;
  Control *control;
  uint     control_id;
};

typedef bool (*EventCallback)(EventInfo &info);

class Window {
public:
HWND    hwnd, hwnd_resize;
HMENU   menu;
HBRUSH  backbrush;
Window *parent;
uint    id;

uint      control_count;
Control **list;
uint      menu_count;
uint     *menu_list;

struct State {
  string classname, title, style;
  uint   x, y, width, height;
  uint   window_width, window_height;
  uint   ws, es;

  bool   dragmove;

  bool   use_icon;
  uint   icon_id;
} state;
  long WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
  bool Add(Control &control);
  Window();

  void SetAlphaLevel(uint level);
  void SetBackgroundColor(uint r, uint g, uint b);
  void SetIcon(uint resource_id);
  bool Create(Window *parent_window, const char *classname, const char *style, uint x, uint y, uint width, uint height, const char *title = "");
  void SetStyle(const char *style);
  void Resize(uint width, uint height, bool center = false);
  void Resize();
  void Move(uint x, uint y);
  void MoveToTop();
  void MoveToBottom();
  void MoveToLeft();
  void MoveToRight();
  void Center();

  virtual void Show(bool do_show);
  virtual bool Visible();
  virtual void Show();
  virtual void Hide();
  virtual void Focus();

struct {
  HMENU  handle;
  char  *text;
} submenu[16];
uint  submenu_pos;
  void CreateMenu();
  void AddMenuGroup(const char *text);
  void EndMenuGroup();
  void AddMenuItem(uint id, const char *text);
  void AddMenuSeparator();
  void CheckMenuItem(uint id, bool do_check);
  bool MenuItemChecked(uint id);
  void CheckMenuItem(uint id);
  void UncheckMenuItem(uint id);
  void ShowMenu(bool state);
  bool MenuVisible();
  void ShowMenu();
  void HideMenu();

EventCallback event_callback;
  void SetEventCallback(EventCallback ecb);
  virtual bool Event(EventInfo &info);
};
