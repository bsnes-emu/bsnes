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
