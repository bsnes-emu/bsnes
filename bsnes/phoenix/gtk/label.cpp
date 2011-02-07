void Label::setParent(Layout &parent) {
}

void Label::setText(const string &text) {
  gtk_label_set_text(GTK_LABEL(object->widget), text);
}

Label::Label() {
  object->widget = gtk_label_new("");
  gtk_misc_set_alignment(GTK_MISC(object->widget), 0.0, 0.5);
}
