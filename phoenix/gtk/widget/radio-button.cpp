namespace phoenix {

static void RadioButton_activate(GtkToggleButton* toggleButton, RadioButton* self) {
  self->p.onActivate();
}

Size pRadioButton::minimumSize() {
  Size size = pFont::size(widget.state.font, radioButton.state.text);

  if(radioButton.state.orientation == Orientation::Horizontal) {
    size.width += radioButton.state.image.width;
    size.height = max(radioButton.state.image.height, size.height);
  }

  if(radioButton.state.orientation == Orientation::Vertical) {
    size.width = max(radioButton.state.image.width, size.width);
    size.height += radioButton.state.image.height;
  }

  return {size.width + 24, size.height + 12};
}

void pRadioButton::setChecked() {
  parent().locked = true;
  for(auto& item : radioButton.state.group) {
    bool checked = &item == &radioButton;
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(item.p.gtkWidget), item.state.checked = checked);
  }
  parent().locked = false;
}

void pRadioButton::setGroup(const group<RadioButton>& group) {
  parent().locked = true;
  for(auto& item : radioButton.state.group) {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(item.p.gtkWidget), item.state.checked);
  }
  parent().locked = false;
}

void pRadioButton::setImage(const image& image, Orientation orientation) {
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

void pRadioButton::setText(string text) {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
  setFont(widget.state.font);
}

void pRadioButton::constructor() {
  gtkWidget = gtk_toggle_button_new();
  g_signal_connect(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(RadioButton_activate), (gpointer)&radioButton);

  setGroup(radioButton.state.group);
  setText(radioButton.state.text);
}

void pRadioButton::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pRadioButton::orphan() {
  destructor();
  constructor();
}

void pRadioButton::onActivate() {
  if(parent().locked) return;
  bool wasChecked = radioButton.state.checked;
  setChecked();
  if(!wasChecked) {
    if(radioButton.onActivate) radioButton.onActivate();
  }
}

pRadioButton& pRadioButton::parent() {
  if(radioButton.state.group.size()) return radioButton.state.group.first().p;
  return *this;
}

}
