class pWindow : public pWidget {
public:
  void create(uint style, uint width, uint height, const char *text = "");
  void close();
  void move(uint x, uint y);
  void resize(uint width, uint height);
  void focus();
  bool focused();
  void fullscreen();
  void unfullscreen();
  uint get_width();
  uint get_height();
  void set_opacity(uint8_t opacity);
  void set_background_color(uint8_t r, uint8_t g, uint8_t b);
  void set_icon(unsigned width, unsigned height, const uint32_t *data);
  void set_text(const char *text = "");
  void attach(Window &window, uint x, uint y);
  void attach(MenuGroup &menugroup);
  void attach(FormControl &formcontrol, uint x, uint y);
  void move(Window &window, uint x, uint y);
  void move(FormControl &formcontrol, uint x, uint y);

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

  /* internal */
  pWindow *owner; //0 = no owner (default)
  GtkWidget *window;
  GtkWidget *menubar;
  GtkWidget *statusbar;
  GtkWidget *menucontainer;
  GtkWidget *formcontainer;
  GtkWidget *statuscontainer;
  GtkWidget* gtk_handle();
  struct {
    bool is_fullscreen;
    uint width;
    uint height;
    uint alpha;
  } state;

  void menu_show(bool = true);
  void menu_hide();
  bool menu_visible();

  void status_set_text(const char *text = "");
  void status_show(bool = true);
  void status_hide();
  bool status_visible();
};
