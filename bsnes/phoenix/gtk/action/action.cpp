static void Action_setFont(GtkWidget *widget, gpointer font) {
  if(font == 0) return;
  gtk_widget_modify_font(widget, (PangoFontDescription*)font);
  if(GTK_IS_CONTAINER(widget)) {
    gtk_container_foreach(GTK_CONTAINER(widget), (GtkCallback)Action_setFont, (PangoFontDescription*)font);
  }
}

void pAction::setEnabled(bool enabled) {
  gtk_widget_set_sensitive(widget, enabled);
}

void pAction::setFont(Font &font) {
  Action_setFont(widget, font.p.gtkFont);
}

void pAction::setVisible(bool visible) {
  gtk_widget_set_visible(widget, visible);
}

void pAction::constructor() {
}
