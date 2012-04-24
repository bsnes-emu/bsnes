Geometry pLabel::minimumGeometry() {
  Geometry geometry = pFont::geometry(widget.state.font, label.state.text);
  return { 0, 0, geometry.width, geometry.height };
}

void pLabel::setText(const string &text) {
  gtk_label_set_text(GTK_LABEL(gtkWidget), text);
}

void pLabel::constructor() {
  gtkWidget = gtk_label_new("");
  gtk_misc_set_alignment(GTK_MISC(gtkWidget), 0.0, 0.5);

  setText(label.state.text);
}

void pLabel::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pLabel::orphan() {
  destructor();
  constructor();
}
