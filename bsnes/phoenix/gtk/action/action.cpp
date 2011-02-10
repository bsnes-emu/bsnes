static void Action_setFont(GtkWidget *widget, gpointer font) {
  if(font) {
    gtk_widget_modify_font(widget, (PangoFontDescription*)font);
    if(GTK_IS_CONTAINER(widget)) {
      gtk_container_foreach(GTK_CONTAINER(widget), (GtkCallback)Action_setFont, (PangoFontDescription*)font);
    }
  }
}

bool Action::visible() {
  return gtk_widget_get_visible(object->widget);
}

void Action::setVisible(bool visible) {
  gtk_widget_set_visible(object->widget, visible);
}

bool Action::enabled() {
  return gtk_widget_get_sensitive(object->widget);
}

void Action::setEnabled(bool enabled) {
  gtk_widget_set_sensitive(object->widget, enabled);
}

Action::Action() {
  action = new Action::Data;
  action->font = 0;
}
