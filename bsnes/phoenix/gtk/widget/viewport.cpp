uintptr_t pViewport::handle() {
  return GDK_WINDOW_XID(gtk_widget_get_window(gtkWidget));
}

void pViewport::constructor() {
  gtkWidget = gtk_drawing_area_new();
//gtk_widget_set_double_buffered(gtkWidget, false);

  GdkColor color;
  color.pixel = 0;
  color.red = 0;
  color.green = 0;
  color.blue = 0;
  gtk_widget_modify_bg(gtkWidget, GTK_STATE_NORMAL, &color);
}

void pViewport::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pViewport::orphan() {
  destructor();
  constructor();
}
