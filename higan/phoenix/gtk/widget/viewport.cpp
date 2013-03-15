namespace phoenix {

static gboolean Viewport_mouseLeave(GtkWidget *widget, GdkEventButton *event, pViewport *self) {
  if(self->viewport.onMouseLeave) self->viewport.onMouseLeave();
  return true;
}

static gboolean Viewport_mouseMove(GtkWidget *widget, GdkEventButton *event, pViewport *self) {
  if(self->viewport.onMouseMove) self->viewport.onMouseMove({ (signed)event->x, (signed)event->y });
  return true;
}

static gboolean Viewport_mousePress(GtkWidget *widget, GdkEventButton *event, pViewport *self) {
  if(self->viewport.onMousePress) switch(event->button) {
  case 1: self->viewport.onMousePress(Mouse::Button::Left); break;
  case 2: self->viewport.onMousePress(Mouse::Button::Middle); break;
  case 3: self->viewport.onMousePress(Mouse::Button::Right); break;
  }
  return true;
}

static gboolean Viewport_mouseRelease(GtkWidget *widget, GdkEventButton *event, pViewport *self) {
  if(self->viewport.onMouseRelease) switch(event->button) {
  case 1: self->viewport.onMouseRelease(Mouse::Button::Left); break;
  case 2: self->viewport.onMouseRelease(Mouse::Button::Middle); break;
  case 3: self->viewport.onMouseRelease(Mouse::Button::Right); break;
  }
  return true;
}

uintptr_t pViewport::handle() {
  return GDK_WINDOW_XID(gtk_widget_get_window(gtkWidget));
}

void pViewport::constructor() {
  gtkWidget = gtk_drawing_area_new();
//gtk_widget_set_double_buffered(gtkWidget, false);
  gtk_widget_add_events(gtkWidget,
    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_POINTER_MOTION_MASK);
  g_signal_connect(G_OBJECT(gtkWidget), "button_press_event", G_CALLBACK(Viewport_mousePress), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "button_release_event", G_CALLBACK(Viewport_mouseRelease), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "leave_notify_event", G_CALLBACK(Viewport_mouseLeave), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "motion_notify_event", G_CALLBACK(Viewport_mouseMove), (gpointer)this);

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

}
