void pLabel::create(unsigned style, unsigned width, unsigned height, const char *text) {
  label = gtk_label_new(text ? text : "");
  set_default_font(label);
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
  gtk_widget_set_size_request(label, width, height);
  gtk_widget_show(label);
}

void pLabel::set_text(const char *text) {
  if(!label) return;
  gtk_label_set_label(GTK_LABEL(label), text ? text : "");
}

pLabel::pLabel(Label &self_) : pFormControl(self_), self(self_) {
  label = 0;
}

/* internal */

GtkWidget* pLabel::gtk_handle() {
  return label;
}
