void pLabel::setText(const string &text) {
  gtk_label_set_text(GTK_LABEL(gtkWidget), text);
}

void pLabel::constructor() {
  gtkWidget = gtk_label_new("");
  gtk_misc_set_alignment(GTK_MISC(gtkWidget), 0.0, 0.5);
}
