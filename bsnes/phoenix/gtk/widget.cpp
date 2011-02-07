static void Widget_setFont(GtkWidget *widget, gpointer font) {
  gtk_widget_modify_font(widget, (PangoFontDescription*)font);
  if(GTK_IS_CONTAINER(widget)) {
    gtk_container_foreach(GTK_CONTAINER(widget), (GtkCallback)Widget_setFont, font);
  }
}

void Widget::setFont(Font &font) {
  Widget_setFont(object->widget, font.font->font);
}

bool Widget::visible() {
  return gtk_widget_get_visible(object->widget);
}

void Widget::setVisible(bool visible) {
  if(visible) gtk_widget_show(object->widget);
  else gtk_widget_hide(object->widget);
}

bool Widget::enabled() {
  return gtk_widget_get_sensitive(object->widget);
}

void Widget::setEnabled(bool enabled) {
  gtk_widget_set_sensitive(object->widget, enabled);
}

bool Widget::focused() {
  return gtk_widget_is_focus(object->widget);
}

void Widget::setFocused() {
  if(visible() == false) setVisible(true);
  gtk_widget_grab_focus(object->widget);
}

Geometry Widget::geometry() {
  return { widget->x, widget->y, widget->width, widget->height };
}

void Widget::setGeometry(unsigned x, unsigned y, unsigned width, unsigned height) {
  if(widget->parent == 0) return;

  widget->x = x;
  widget->y = y;
  widget->width = width;
  widget->height = height;

  gtk_fixed_move(GTK_FIXED(widget->parent->object->formContainer), object->widget, x, y);
  gtk_widget_set_size_request(object->widget, width, height);
}

Widget::Widget() {
  widget = new Widget::Data;
  widget->parent = 0;
  widget->x = 0;
  widget->y = 0;
  widget->width = 0;
  widget->height = 0;
}
