#if defined(Hiro_RadioButton)

namespace hiro {

static auto RadioButton_activate(GtkToggleButton*, pRadioButton* p) -> void {
  if(p->_parent().locked()) return;
  bool wasChecked = p->state().checked;
  p->setChecked();
  if(!wasChecked) p->self().doActivate();
}

auto pRadioButton::construct() -> void {
  gtkWidget = gtk_toggle_button_new();

  setGroup(state().group);
  setBordered(state().bordered);
  setIcon(state().icon);
  setOrientation(state().orientation);
  setText(state().text);

  g_signal_connect(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(RadioButton_activate), (gpointer)this);

  pWidget::construct();
}

auto pRadioButton::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pRadioButton::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);

  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(size.width() + state().icon.width);
    size.setHeight(max(size.height(), state().icon.height));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(size.width(), state().icon.width));
    size.setHeight(size.height() + state().icon.height);
  }

  return {size.width() + 24, size.height() + 12};
}

auto pRadioButton::setBordered(bool bordered) -> void {
  gtk_button_set_relief(GTK_BUTTON(gtkWidget), bordered ? GTK_RELIEF_NORMAL : GTK_RELIEF_NONE);
}

auto pRadioButton::setChecked() -> void {
  _parent().lock();
  for(auto& weak : state().group) {
    if(auto item = weak.acquire()) {
      if(item->self()) {
        bool checked = item->self() == this;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(item->self()->gtkWidget), item->state.checked = checked);
      }
    }
  }
  _parent().unlock();
}

auto pRadioButton::setGroup(const vector<wRadioButton>& group) -> void {
  _parent().lock();
  for(auto& weak : state().group) {
    if(auto item = weak.acquire()) {
      if(item->self()) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(item->self()->gtkWidget), item->checked());
    }
  }
  _parent().unlock();
}

auto pRadioButton::setIcon(const image& icon) -> void {
  if(icon) {
    GtkImage* gtkImage = CreateImage(icon);
    gtk_button_set_image(GTK_BUTTON(gtkWidget), (GtkWidget*)gtkImage);
  } else {
    gtk_button_set_image(GTK_BUTTON(gtkWidget), nullptr);
  }
}

auto pRadioButton::setOrientation(Orientation orientation) -> void {
  switch(orientation) {
  case Orientation::Horizontal: gtk_button_set_image_position(GTK_BUTTON(gtkWidget), GTK_POS_LEFT); break;
  case Orientation::Vertical:   gtk_button_set_image_position(GTK_BUTTON(gtkWidget), GTK_POS_TOP);  break;
  }
}

auto pRadioButton::setText(const string& text) -> void {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
  setFont(self().font(true));  //gtk_button_set_label() recreates label, which destroys currently assigned font
}

auto pRadioButton::_parent() -> pRadioButton& {
  if(state().group.size()) {
    if(auto item = state().group.first().acquire()) {
      if(item->self()) return *item->self();
    }
  }
  return *this;
}

}

#endif
