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
  void set_background_color(uint8 r, uint8 g, uint8 b);
  void set_text(const char *text = "");
  void attach(Window &window, uint x, uint y);
  void attach(MenuGroup &menugroup);
  void attach(FormControl &formcontrol, uint x, uint y);
  void move(Window &window, uint x, uint y);
  void move(FormControl &formcontrol, uint x, uint y);
  void menu_show(bool state = true);
  void menu_hide();
  bool menu_visible();

  Window &self;
  pWindow(Window&);

/* internal */
  pWindow *owner; //0 = no owner (default)
  GtkWidget *window;
  GtkWidget *menubar;
  GtkWidget *menucontainer;
  GtkWidget *formcontainer;
  GtkWidget* gtk_handle();
};
