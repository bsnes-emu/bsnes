void pFrame::create(uint style, uint width, uint height, const char *text) {
  frame = gtk_frame_new(text ? text : "");
  set_default_font(frame);
  gtk_widget_set_size_request(frame, width, height);
  gtk_widget_show(frame);
}

void pFrame::set_text(const char *text) {
  gtk_frame_set_label(GTK_FRAME(frame), text ? text : "");
}

pFrame::pFrame(Frame &self_) : pFormControl(self_), self(self_) {
  frame = 0;
}

/* internal */

GtkWidget* pFrame::gtk_handle() {
  return frame;
}
