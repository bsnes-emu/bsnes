static void Button_activate(Button *self) {
  if(self->onActivate) self->onActivate();
}

Geometry pButton::minimumGeometry() {
  Geometry geometry = pFont::geometry(widget.state.font, button.state.text);

  if(button.state.orientation == Orientation::Horizontal) {
    geometry.width += button.state.image.width;
    geometry.height = max(button.state.image.height, geometry.height);
  }

  if(button.state.orientation == Orientation::Vertical) {
    geometry.width = max(button.state.image.width, geometry.width);
    geometry.height += button.state.image.height;
  }

  return { 0, 0, geometry.width + 24, geometry.height + 12 };
}

void pButton::setImage(const image &image, Orientation orientation) {
  GtkImage *gtkImage = CreateImage(image);
  gtk_button_set_image(GTK_BUTTON(gtkWidget), (GtkWidget*)gtkImage);
  switch(orientation) {
  case Orientation::Horizontal: gtk_button_set_image_position(GTK_BUTTON(gtkWidget), GTK_POS_LEFT); break;
  case Orientation::Vertical:   gtk_button_set_image_position(GTK_BUTTON(gtkWidget), GTK_POS_TOP);  break;
  }
}

void pButton::setText(const string &text) {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
  setFont(widget.state.font);
}

void pButton::constructor() {
  gtkWidget = gtk_button_new();
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "clicked", G_CALLBACK(Button_activate), (gpointer)&button);

  setText(button.state.text);
}

void pButton::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pButton::orphan() {
  destructor();
  constructor();
}
