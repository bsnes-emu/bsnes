class pWindow : public pWidget {
public:
  void create(unsigned style, unsigned width, unsigned height, const char *text = "");
  void close();
  void move(unsigned x, unsigned y);
  void resize(unsigned width, unsigned height);
  void focus();
  bool focused();
  void fullscreen();
  void unfullscreen();
  unsigned get_width();
  unsigned get_height();
  void set_opacity(uint8_t opacity);
  void set_background_color(uint8_t r, uint8_t g, uint8_t b);
  void set_icon(unsigned width, unsigned height, const uint32_t *data);
  void set_text(const char *text = "");
  void attach(Window &window, unsigned x, unsigned y);
  void attach(MenuGroup &menugroup);
  void attach(FormControl &formcontrol, unsigned x, unsigned y);
  void move(Window &window, unsigned x, unsigned y);
  void move(FormControl &formcontrol, unsigned x, unsigned y);

  class Statusbar {
  public:
    void set_text(const char *text = "");
    void show(bool = true);
    void hide();
    bool visible();

    pWindow &p;
    Statusbar(pWindow&);
  } status;

  class Menubar {
  public:
    void show(bool = true);
    void hide();
    bool visible();

    pWindow &p;
    Menubar(pWindow&);
  } menu;

  Window &self;
  pWindow(Window&);
  ~pWindow();

  /* internal */
  HWND hwnd;
  HWND hwndr; //hidden window, used as resize assistant
  HICON hicon;
  HMENU hmenu;
  HWND hstatus;
  HBRUSH background;
  uint8_t opacity;
  bool is_fullscreen;
  bool auto_center;
  unsigned width, height;

  uintptr_t handle();

  void show(bool = true);
  void hide();
  bool visible();

  void menu_show(bool = true);
  void menu_hide();
  bool menu_visible();

  void status_set_text(const char *text = "");
  void status_show(bool = true);
  void status_hide();
  bool status_visible();
};
