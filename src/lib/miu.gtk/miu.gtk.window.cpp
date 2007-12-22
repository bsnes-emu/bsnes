gint miu_pwindow_close(pWindow *p) {
uintptr_t r = p->self.on_close ? p->self.on_close(Event(Event::Close, 0, &p->self)) : true;
  return !bool(r);
}

gint miu_pwindow_keydown(GtkWidget *w, GdkEventKey *key, pWindow *p) {
  if(p && p->self.on_keydown) p->self.on_keydown(Event(Event::KeyDown, pmiu().translate_key(key->keyval), &p->self));
  return FALSE;
}

gint miu_pwindow_keyup(GtkWidget *w, GdkEventKey *key, pWindow *p) {
  if(p && p->self.on_keyup) p->self.on_keyup(Event(Event::KeyUp, pmiu().translate_key(key->keyval), &p->self));
  return FALSE;
}

void pWindow::create(uint style, uint width, uint height, const char *text) {
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), text ? text : "");
  gtk_window_set_resizable(GTK_WINDOW(window), false);
  if(style & Window::AutoCenter) gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
  g_signal_connect_swapped(G_OBJECT(window), "delete_event", G_CALLBACK(miu_pwindow_close), (gpointer)this);
  g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(miu_pwindow_keydown), (gpointer)this);
  g_signal_connect(G_OBJECT(window), "key_release_event", G_CALLBACK(miu_pwindow_keyup), (gpointer)this);

  menucontainer = gtk_vbox_new(false, 0);
  gtk_container_add(GTK_CONTAINER(window), menucontainer);
  gtk_widget_show(menucontainer);

  menubar = gtk_menu_bar_new();
  gtk_box_pack_start(GTK_BOX(menucontainer), menubar, false, false, 0);

  formcontainer = gtk_fixed_new();
  gtk_widget_set_size_request(formcontainer, width, height);
  gtk_box_pack_end(GTK_BOX(menucontainer), formcontainer, true, true, 0);
  gtk_widget_show(formcontainer);
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
}

void pWindow::focus() {
  gtk_window_present(GTK_WINDOW(window));
}

bool pWindow::focused() {
  return gtk_window_is_active(GTK_WINDOW(window));
}

void pWindow::fullscreen() {
  gtk_window_fullscreen(GTK_WINDOW(window));
}

void pWindow::unfullscreen() {
  gtk_window_unfullscreen(GTK_WINDOW(window));
}

void pWindow::set_background_color(uint8 r, uint8 g, uint8 b) {
GdkColor color;
  color.pixel = (r << 16) | (g << 8) | b;
  color.red   = (r << 8) | r;
  color.green = (g << 8) | g;
  color.blue  = (b << 8) | b;
  gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);
  gtk_widget_modify_bg(formcontainer, GTK_STATE_NORMAL, &color);
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

void pWindow::menu_show(bool state) {
  if(!menubar) return;
  state ? gtk_widget_show(menubar) : gtk_widget_hide(menubar);
}

void pWindow::menu_hide() {
  menu_show(false);
}

bool pWindow::menu_visible() {
  return GTK_WIDGET_VISIBLE(menubar);
}

pWindow::pWindow(Window &self_) : pWidget(self_), self(self_) {
  owner = 0;
  window = 0;
  menubar = 0;
  menucontainer = 0;
  formcontainer = 0;
}

/* internal */

GtkWidget* pWindow::gtk_handle() {
  return owner ? formcontainer : window;
}
