namespace phoenix {

static void Button_activate(Button* self) {
  if(self->onActivate) self->onActivate();
}

Size pButton::minimumSize() {
  Size size = pFont::size(widget.state.font, button.state.text);

  if(button.state.orientation == Orientation::Horizontal) {
    size.width += button.state.image.width;
    size.height = max(button.state.image.height, size.height);
  }

  if(button.state.orientation == Orientation::Vertical) {
    size.width = max(button.state.image.width, size.width);
    size.height += button.state.image.height;
  }

  return {size.width + 24, size.height + 12};
}

void pButton::setImage(const image& image, Orientation orientation) {
  if(image.empty() == false) {
    GtkImage* gtkImage = CreateImage(image);
    gtk_button_set_image(GTK_BUTTON(gtkWidget), (GtkWidget*)gtkImage);
  } else {
    gtk_button_set_image(GTK_BUTTON(gtkWidget), nullptr);
  }
  switch(orientation) {
  case Orientation::Horizontal: gtk_button_set_image_position(GTK_BUTTON(gtkWidget), GTK_POS_LEFT); break;
  case Orientation::Vertical:   gtk_button_set_image_position(GTK_BUTTON(gtkWidget), GTK_POS_TOP);  break;
  }
}

void pButton::setText(string text) {
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

}
