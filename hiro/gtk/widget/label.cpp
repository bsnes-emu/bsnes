#if defined(Hiro_Label)

namespace hiro {

static auto Label_draw(GtkWidget* widget, cairo_t* context, pLabel* p) -> int {
  auto color = p->state().backgroundColor;
  if(auto window = p->self().parentWindow(true)) {
    if(!color) color = window->backgroundColor();
  }

  if(color) {
    double red   = (double)color.red()   / 255.0;
    double green = (double)color.green() / 255.0;
    double blue  = (double)color.blue()  / 255.0;
    double alpha = (double)color.alpha() / 255.0;

    if(gdk_screen_is_composited(gdk_screen_get_default())
    && gdk_screen_get_rgba_visual(gdk_screen_get_default())
    ) {
      cairo_set_source_rgba(context, red, green, blue, alpha);
    } else {
      cairo_set_source_rgb(context, red, green, blue);
    }

    cairo_set_operator(context, CAIRO_OPERATOR_SOURCE);
    cairo_paint(context);
  } else {
    #if HIRO_GTK==3
    auto style = gtk_widget_get_style_context(widget);
    if(auto tabFrame = p->self().parentTabFrame(true)) {
      if(auto self = tabFrame->self()) style = gtk_widget_get_style_context(self->gtkWidget);
    }
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    gtk_render_background(style, context, 0, 0, allocation.width, allocation.height);
    #endif
  }

  return false;
}

static auto Label_expose(GtkWidget* widget, GdkEvent* event, pLabel* p) -> int {
  cairo_t* context = gdk_cairo_create(gtk_widget_get_window(widget));
  Label_draw(widget, context, p);
  cairo_destroy(context);
  return false;
}

static auto Label_mouseEnter(GtkWidget* widget, GdkEventButton* event, pCanvas* p) -> signed {
  Widget_mouseEnter(widget, event, p);
  return true;
}

static auto Label_mouseLeave(GtkWidget* widget, GdkEventButton* event, pCanvas* p) -> signed {
  Widget_mouseLeave(widget, event, p);
  return true;
}

static auto Label_mousePress(GtkWidget* widget, GdkEventButton* event, pLabel* p) -> int {
  switch(event->button) {
  case 1: p->self().doMousePress(Mouse::Button::Left); break;
  case 2: p->self().doMousePress(Mouse::Button::Middle); break;
  case 3: p->self().doMousePress(Mouse::Button::Right); break;
  }
  return true;
}

static auto Label_mouseRelease(GtkWidget* widget, GdkEventButton* event, pLabel* p) -> int {
  switch(event->button) {
  case 1: p->self().doMouseRelease(Mouse::Button::Left); break;
  case 2: p->self().doMouseRelease(Mouse::Button::Middle); break;
  case 3: p->self().doMouseRelease(Mouse::Button::Right); break;
  }
  return true;
}

auto pLabel::construct() -> void {
  gtkWidget = gtk_event_box_new();
  subWidget = gtk_label_new("");
  gtk_widget_show(subWidget);
  gtk_container_add(GTK_CONTAINER(gtkWidget), subWidget);

  setAlignment(state().alignment);
  setBackgroundColor(state().backgroundColor);
  setForegroundColor(state().foregroundColor);
  setText(state().text);

  g_signal_connect(G_OBJECT(gtkWidget), "button-press-event", G_CALLBACK(Label_mousePress), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "button-release-event", G_CALLBACK(Label_mouseRelease), (gpointer)this);
  #if HIRO_GTK==2
  g_signal_connect(G_OBJECT(subWidget), "expose-event", G_CALLBACK(Label_expose), (gpointer)this);
  #elif HIRO_GTK==3
  g_signal_connect(G_OBJECT(subWidget), "draw", G_CALLBACK(Label_draw), (gpointer)this);
  #endif
  g_signal_connect(G_OBJECT(gtkWidget), "enter-notify-event", G_CALLBACK(Label_mouseEnter), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "leave-notify-event", G_CALLBACK(Label_mouseLeave), (gpointer)this);

  pWidget::construct();
}

auto pLabel::destruct() -> void {
  gtk_widget_destroy(subWidget);
  gtk_widget_destroy(gtkWidget);
}

auto pLabel::minimumSize() const -> Size {
  auto size = pFont::size(self().font(true), state().text);
  return {size.width(), size.height()};
}

auto pLabel::setAlignment(Alignment alignment) -> void {
  if(!alignment) alignment = {0.0, 0.5};
  gtk_misc_set_alignment(GTK_MISC(subWidget), alignment.horizontal(), alignment.vertical());
  auto justify = GTK_JUSTIFY_CENTER;
  if(alignment.horizontal() < 0.333) justify = GTK_JUSTIFY_LEFT;
  if(alignment.horizontal() > 0.666) justify = GTK_JUSTIFY_RIGHT;
  gtk_label_set_justify(GTK_LABEL(subWidget), justify);
}

auto pLabel::setBackgroundColor(Color color) -> void {
  //GTK3 will paint the wrong background color when a label is inside of a TabFrame
  //this is caused by the GtkEventBox wrapper that is required to paint custom backgrounds
  //handle background painting via "draw" or "expose-event" signals instead
}

auto pLabel::setForegroundColor(Color color) -> void {
  auto gdkColor = CreateColor(color);
  gtk_widget_modify_fg(subWidget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pLabel::setText(const string& text) -> void {
  gtk_label_set_text(GTK_LABEL(subWidget), text);
}

auto pLabel::setVisible(bool visible) -> void {
  setBackgroundColor(state().backgroundColor);
  return pWidget::setVisible(visible);
}

}

#endif
