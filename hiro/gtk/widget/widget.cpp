#if defined(Hiro_Widget)

namespace hiro {

static auto Widget_drop(GtkWidget* widget, GdkDragContext* context, int x, int y,
GtkSelectionData* data, uint type, uint timestamp, pWidget* p) -> void {
  if(!p->state().droppable) return;
  auto paths = DropPaths(data);
  if(!paths) return;
  p->self().doDrop(paths);
}

static auto Widget_mouseEnter(GtkWidget* widget, GdkEventButton* event, pWidget* p) -> int {
  if(auto cursor = p->gdkMouseCursor) {
    if(auto window = gtk_widget_get_parent_window(widget)) {
      gdk_window_set_cursor(window, cursor);
    }
  }
  p->self().doMouseEnter();
  return true;
}

static auto Widget_mouseLeave(GtkWidget* widget, GdkEventButton* event, pWidget* p) -> int {
  if(auto cursor = p->gdkMouseCursor) {
    if(auto window = gtk_widget_get_parent_window(widget)) {
      gdk_window_set_cursor(window, nullptr);
    }
  }
  p->self().doMouseLeave();
  return true;
}

static auto Widget_mouseMove(GtkWidget* widget, GdkEventButton* event, pWidget* p) -> signed {
  p->self().doMouseMove({(signed)event->x, (signed)event->y});
  return true;
}

static auto Widget_mousePress(GtkWidget* widget, GdkEventButton* event, pWidget* p) -> signed {
  switch(event->button) {
  case 1: p->self().doMousePress(Mouse::Button::Left); break;
  case 2: p->self().doMousePress(Mouse::Button::Middle); break;
  case 3: p->self().doMousePress(Mouse::Button::Right); break;
  }
  return true;
}

static auto Widget_mouseRelease(GtkWidget* widget, GdkEventButton* event, pWidget* p) -> signed {
  switch(event->button) {
  case 1: p->self().doMouseRelease(Mouse::Button::Left); break;
  case 2: p->self().doMouseRelease(Mouse::Button::Middle); break;
  case 3: p->self().doMouseRelease(Mouse::Button::Right); break;
  }
  return true;
}

auto pWidget::construct() -> void {
  if(!gtkWidget) return;
  if(auto window = self().parentWindow(true)) {
    if(window->self()) window->self()->_append(self());
    setDroppable(self().droppable());
    setEnabled(self().enabled(true));
    setFocusable(self().focusable());
    setFont(self().font(true));
    setMouseCursor(self().mouseCursor());
    setToolTip(self().toolTip());
    setVisible(self().visible(true));
  }
}

auto pWidget::destruct() -> void {
}

auto pWidget::container(mWidget& widget) -> GtkWidget* {
  return nullptr;
}

auto pWidget::focused() const -> bool {
  if(!gtkWidget) return false;
  return gtk_widget_has_focus(gtkWidget);
}

auto pWidget::setDroppable(bool droppable) -> void {
  if(!gtkWidget) return;
  if(droppable) {
    gtk_drag_dest_set(gtkWidget, GTK_DEST_DEFAULT_ALL, nullptr, 0, GDK_ACTION_COPY);
    gtk_drag_dest_add_uri_targets(gtkWidget);
  }
}

auto pWidget::setEnabled(bool enabled) -> void {
  if(!gtkWidget) return;
  gtk_widget_set_sensitive(gtkWidget, enabled);
}

auto pWidget::setFocusable(bool focusable) -> void {
  //virtual overload: only configurable for Canvas and Viewport
}

auto pWidget::setFocused() -> void {
  if(!gtkWidget) return;
  gtk_widget_grab_focus(gtkWidget);
}

auto pWidget::setFont(const Font& font) -> void {
  if(!gtkWidget) return;
  return pFont::setFont(gtkWidget, font);
}

auto pWidget::setGeometry(Geometry geometry) -> void {
  if(!gtkWidget) return;
  if(gtkParent) gtk_fixed_move(GTK_FIXED(gtkParent), gtkWidget, geometry.x(), geometry.y());
  if(geometry.width()  < 1) geometry.setWidth (1);
  if(geometry.height() < 1) geometry.setHeight(1);
  gtk_widget_set_size_request(gtkWidget, geometry.width(), geometry.height());
  if(0 && gtk_widget_get_realized(gtkWidget)) {
    static bool locked = false;
    if(!locked) {
      locked = true;
      auto time = chrono::millisecond();
      while(chrono::millisecond() - time < 20) {
        GtkAllocation allocation;
        gtk_widget_get_allocation(gtkWidget, &allocation);
        if(allocation.width == geometry.width() && allocation.height == geometry.height()) break;
        gtk_main_iteration_do(false);
      }
      locked = false;
    }
  }
  pSizable::setGeometry(geometry);
}

auto pWidget::setMouseCursor(const MouseCursor& mouseCursor) -> void {
//TODO: this should be freed, yet this code seems to break the cursor assignment completely ...
//if(gdkMouseCursor) {
//  gdk_cursor_unref(gdkMouseCursor);
//  gdkMouseCursor = nullptr;
//}

  if(mouseCursor) {
    string name;
    if(mouseCursor == MouseCursor::Hand) name = "hand1";
    if(mouseCursor == MouseCursor::HorizontalResize) name = "sb_h_double_arrow";
    if(mouseCursor == MouseCursor::VerticalResize) name = "sb_v_double_arrow";
    if(name) {
      gdkMouseCursor = gdk_cursor_new_from_name(gdk_display_get_default(), name);
    }
  }
}

auto pWidget::setToolTip(const string& toolTip) -> void {
  gtk_widget_set_tooltip_text(gtkWidget, toolTip);
}

auto pWidget::setVisible(bool visible) -> void {
  if(!gtkWidget) return;
  gtk_widget_set_visible(gtkWidget, visible);
}

}

#endif
