static gint hiro_pwindow_close(pWindow *p) {
  uintptr_t r = p->self.on_close ? p->self.on_close(Event(Event::Close, 0, &p->self)) : true;
  return !bool(r);
}

static gboolean hiro_pwindow_expose(pWindow *p) {
  cairo_t *context = gdk_cairo_create(p->window->window);
  GtkStyle *style = gtk_widget_get_style(p->window);

  double red   = double(style->bg[GTK_STATE_NORMAL].red)   / 65536.0;
  double green = double(style->bg[GTK_STATE_NORMAL].green) / 65536.0;
  double blue  = double(style->bg[GTK_STATE_NORMAL].blue)  / 65536.0;
  double alpha = double(p->state.alpha) / 256.0;

  if(phiro().is_composited == true) {
    cairo_set_source_rgba(context, red, green, blue, alpha);
  } else {
    cairo_set_source_rgb(context, red, green, blue);
  }

  cairo_set_operator(context, CAIRO_OPERATOR_SOURCE);
  cairo_paint(context);
  cairo_destroy(context);
  return FALSE;
}

static gint hiro_pwindow_keydown(GtkWidget *w, GdkEventKey *key, pWindow *p) {
  if(p && p->self.on_keydown) p->self.on_keydown(Event(Event::KeyDown, phiro().translate_key(key->keyval), &p->self));
  return FALSE;
}

static gint hiro_pwindow_keyup(GtkWidget *w, GdkEventKey *key, pWindow *p) {
  if(p && p->self.on_keyup) p->self.on_keyup(Event(Event::KeyUp, phiro().translate_key(key->keyval), &p->self));
  return FALSE;
}

void pWindow::create(uint style, uint width, uint height, const char *text) {
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_colormap(window, phiro().colormap);

  gtk_window_set_title(GTK_WINDOW(window), text ? text : "");
  gtk_window_set_resizable(GTK_WINDOW(window), false);
  gtk_widget_set_app_paintable(window, true);
  if(style & Window::AutoCenter) gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
  g_signal_connect_swapped(G_OBJECT(window), "delete_event", G_CALLBACK(hiro_pwindow_close), (gpointer)this);
  g_signal_connect_swapped(G_OBJECT(window), "expose_event", G_CALLBACK(hiro_pwindow_expose), (gpointer)this);
  g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(hiro_pwindow_keydown), (gpointer)this);
  g_signal_connect(G_OBJECT(window), "key_release_event", G_CALLBACK(hiro_pwindow_keyup), (gpointer)this);

  menucontainer = gtk_vbox_new(false, 0);
  gtk_container_add(GTK_CONTAINER(window), menucontainer);
  gtk_widget_show(menucontainer);

  menubar = gtk_menu_bar_new();
  gtk_box_pack_start(GTK_BOX(menucontainer), menubar, false, false, 0);

  formcontainer = gtk_fixed_new();
  gtk_widget_set_size_request(formcontainer, width, height);
  gtk_box_pack_start(GTK_BOX(menucontainer), formcontainer, true, true, 0);
  gtk_widget_show(formcontainer);

  //GTK+ statusbar background can be transparent, depending upon GTK+ theme
  //therefore, pack statusbar inside an event box, which always has a background
  //this allows pWindow::set_background_color() to change the window color,
  //without affecting the statusbar color
  statuscontainer = gtk_event_box_new();
  statusbar = gtk_statusbar_new();
  gtk_container_add(GTK_CONTAINER(statuscontainer), statusbar);
  gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(statusbar), false);
  gtk_box_pack_start(GTK_BOX(menucontainer), statuscontainer, false, false, 0);
  gtk_widget_show(statuscontainer);

  gtk_widget_realize(window);
  state.is_fullscreen = false;
  state.width = width;
  state.height = height;
}

void pWindow::close() {
  gtk_widget_destroy(window);
}

void pWindow::move(uint x, uint y) {
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_NONE);
  gtk_window_move(GTK_WINDOW(window), x, y);
}

void pWindow::resize(uint width, uint height) {
  gtk_widget_set_size_request(formcontainer, width, height);
  state.width = width;
  state.height = height;
}

void pWindow::focus() {
  gtk_window_present(GTK_WINDOW(window));
}

bool pWindow::focused() {
  return gtk_window_is_active(GTK_WINDOW(window));
}

//gtk_window_(un/)fullscreen() alone does not work well on certain WMs, such as Openbox.
//sometimes, the window will not resize (but will move to the top left.)
//sometimes, the window decorations will not disappear.
//therefore, to be safe, perform some manual window adjustments as well

void pWindow::fullscreen() {
  if(state.is_fullscreen == true) return;
  state.is_fullscreen = true;

  gtk_window_fullscreen(GTK_WINDOW(window));
  gtk_window_set_decorated(GTK_WINDOW(window), false);
  gtk_widget_set_size_request(window, gdk_screen_width(), gdk_screen_height());
}

//note that the size of the window is bound to be at least the size of the formcontainer.
//this is why the window size is set to -1, -1 below, so that it becomes the minimum
//needed to display all of the formcontainer.

void pWindow::unfullscreen() {
  if(state.is_fullscreen == false) return;
  state.is_fullscreen = false;

  gtk_widget_set_size_request(formcontainer, state.width, state.height);
  gtk_widget_set_size_request(window, -1, -1);
  gtk_window_set_decorated(GTK_WINDOW(window), true);
  gtk_window_unfullscreen(GTK_WINDOW(window));
}

//gtk_widget_size_request() on a window immediately after gtk_window_(un/)fullscreen()
//is unreliable, as it will usually report the previous window size.
//therefore, calculate it manually by using state information.

uint pWindow::get_width() {
  if(state.is_fullscreen == false) return state.width;
  return gdk_screen_width();
}

uint pWindow::get_height() {
  if(state.is_fullscreen == false) return state.height;
  uint height = gdk_screen_height();

  //do not include menubar height in client area height
  if(menu.visible()) {
    GtkRequisition req;
    gtk_widget_size_request(menubar, &req);
    height -= req.height;
  }

  //do not include statusbar height in client area height
  if(status.visible()) {
    GtkRequisition req;
    gtk_widget_size_request(statusbar, &req);
    height -= req.height;
  }

  return height;
}

void pWindow::set_opacity(uint8_t opacity) {
  state.alpha = opacity;
  if(window) gtk_widget_queue_draw(window);
}

void pWindow::set_background_color(uint8_t r, uint8_t g, uint8_t b) {
  GdkColor color;
  color.pixel = (r << 16) | (g << 8) | b;
  color.red   = (r << 8) | r;
  color.green = (g << 8) | g;
  color.blue  = (b << 8) | b;
  gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);
}

void pWindow::set_icon(unsigned width, unsigned height, const uint32_t *data) {
  uint8_t *temp = (uint8_t*)malloc(width * height * 4);
  memcpy(temp, data, width * height * 4);
  for(unsigned i = 0; i < width * height; i++) {
    //ABGR -> ARGB
    uint8_t t = temp[i * 4 + 0];
    temp[i * 4 + 0] = temp[i * 4 + 2];
    temp[i * 4 + 2] = t;
  }

  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
    (const guchar*)temp, GDK_COLORSPACE_RGB,
    /* has_alpha = */ TRUE, /* bits_per_sample = */ 8,
    width, height, /* rowstride = */ width * 4,
    /* destroy_fn = */ NULL, /* destroy_fn_data = */ NULL
  );
  gtk_window_set_icon(GTK_WINDOW(window), pixbuf);

  g_object_unref(pixbuf);
  free(temp);
}

void pWindow::set_text(const char *text) {
  gtk_window_set_title(GTK_WINDOW(window), text ? text : "");
}

void pWindow::attach(Window &window, uint x, uint y) {
  window.p.owner = this;

  //GTK+ does not support attaching a window to another window,
  //so instead reparent the container from the child window to
  //the parent window, and reposition the child window's container
  gtk_widget_hide(window.p.window);
  gtk_widget_hide(window.p.formcontainer);
  gtk_widget_reparent(window.p.formcontainer, formcontainer);
  gtk_fixed_move(GTK_FIXED(formcontainer), window.p.formcontainer, x, y);
  gtk_widget_show(window.p.formcontainer);
}

void pWindow::attach(MenuGroup &menugroup) {
  gtk_menu_bar_append(menubar, menugroup.p.item);
  gtk_widget_show(menubar);
}

void pWindow::attach(FormControl &formcontrol, uint x, uint y) {
  gtk_fixed_put(GTK_FIXED(formcontainer), formcontrol.p.gtk_handle(), x, y);
}

void pWindow::move(Window &window, uint x, uint y) {
  gtk_fixed_move(GTK_FIXED(formcontainer), window.p.gtk_handle(), x, y);
}

void pWindow::move(FormControl &formcontrol, uint x, uint y) {
  gtk_fixed_move(GTK_FIXED(formcontainer), formcontrol.p.gtk_handle(), x, y);
}

/* pWindow -> Menubar */

void pWindow::Menubar::show(bool state) {
  p.menu_show(state);
}

void pWindow::Menubar::hide() {
  p.menu_hide();
}

bool pWindow::Menubar::visible() {
  return p.menu_visible();
}

pWindow::Menubar::Menubar(pWindow &p_) : p(p_) {
}

/* pWindow -> Statusbar */

void pWindow::Statusbar::set_text(const char *text) {
  p.status_set_text(text);
}

void pWindow::Statusbar::show(bool state) {
  p.status_show(state);
}

void pWindow::Statusbar::hide() {
  p.status_hide();
}

bool pWindow::Statusbar::visible() {
  return p.status_visible();
}

pWindow::Statusbar::Statusbar(pWindow &p_) : p(p_) {
}

pWindow::pWindow(Window &self_) : pWidget(self_), self(self_), menu(*this), status(*this) {
  owner = 0;
  window = 0;
  menubar = 0;
  statusbar = 0;
  menucontainer = 0;
  formcontainer = 0;
  statuscontainer = 0;

  state.is_fullscreen = false;
  state.width = 0;
  state.height = 0;
  state.alpha = 255;
}

/* internal */

GtkWidget* pWindow::gtk_handle() {
  return owner ? formcontainer : window;
}

void pWindow::menu_show(bool state) {
  state ? gtk_widget_show(menubar) : gtk_widget_hide(menubar);
}

void pWindow::menu_hide() {
  menu_show(false);
}

bool pWindow::menu_visible() {
  return GTK_WIDGET_VISIBLE(menubar);
}

void pWindow::status_set_text(const char *text) {
  gtk_statusbar_pop(GTK_STATUSBAR(statusbar), 1);
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), 1, text ? text : "");
}

void pWindow::status_show(bool state) {
  state ? gtk_widget_show(statusbar) : gtk_widget_hide(statusbar);
}

void pWindow::status_hide() {
  status_show(false);
}

bool pWindow::status_visible() {
  return GTK_WIDGET_VISIBLE(statusbar);
}
