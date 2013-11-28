namespace phoenix {

static void CheckButton_toggle(GtkToggleButton* toggleButton, CheckButton* self) {
  self->p.onToggle();
}

Size pCheckButton::minimumSize() {
  Size size = pFont::size(widget.state.font, checkButton.state.text);

  if(checkButton.state.orientation == Orientation::Horizontal) {
    size.width += checkButton.state.image.width;
    size.height = max(checkButton.state.image.height, size.height);
  }

  if(checkButton.state.orientation == Orientation::Vertical) {
    size.width = max(checkButton.state.image.width, size.width);
    size.height += checkButton.state.image.height;
  }

  return {size.width + 24, size.height + 12};
}

void pCheckButton::setChecked(bool checked) {
  locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget), checked);
  locked = false;
}

void pCheckButton::setImage(const image& image, Orientation orientation) {
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

void pCheckButton::setText(string text) {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
  setFont(widget.state.font);
}

void pCheckButton::constructor() {
  gtkWidget = gtk_toggle_button_new();
  g_signal_connect(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(CheckButton_toggle), (gpointer)&checkButton);

  setChecked(checkButton.state.checked);
  setText(checkButton.state.text);
}

void pCheckButton::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pCheckButton::orphan() {
  destructor();
  constructor();
}

void pCheckButton::onToggle() {
  if(locked) return;
  checkButton.state.checked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkWidget));
  if(checkButton.onToggle) checkButton.onToggle();
}

}
