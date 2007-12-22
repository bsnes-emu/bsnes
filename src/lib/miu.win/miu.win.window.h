class pWindow : public pWidget {
public:
  Window &self;
  void create(uint style, uint width, uint height, const char *text = "");
  void close();
  void move(uint x, uint y);
  void resize(uint width, uint height);
  void focus();
  bool focused();
  void fullscreen();
  void unfullscreen();
  void set_background_color(uint8 r, uint8 g, uint8 b);
  void set_text(const char *text = "");
  void attach(Window &window, uint x, uint y);
  void attach(MenuGroup &menugroup);
  void attach(FormControl &formcontrol, uint x, uint y);
  void move(Window &window, uint x, uint y);
  void move(FormControl &formcontrol, uint x, uint y);
  void menu_show(bool = true);
  void menu_hide();
  bool menu_visible();
  uintptr_t handle();

  pWindow(Window&);
  ~pWindow();

/* internal */
  HWND hwnd;
  HWND hwndr; //hidden window, used as resize assistant
  HMENU menu;
  HBRUSH background;
  bool is_fullscreen;
  bool auto_center;
  uint width, height;

  void show(bool = true);
  void hide();
  bool visible();
};
