namespace hiro {

static auto RadioLabel_activate(GtkToggleButton*, pRadioLabel* p) -> void {
  if(p->_parent().locked()) return;
  bool wasChecked = p->state().checked;
  p->setChecked();
  if(!wasChecked) p->self().doActivate();
}

auto pRadioLabel::construct() -> void {
  gtkWidget = gtk_radio_button_new_with_label(nullptr, "");

  setGroup(state().group);
  setText(state().text);

  g_signal_connect(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(RadioLabel_activate), (gpointer)this);

  pWidget::construct();
}

auto pRadioLabel::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pRadioLabel::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);
  return {size.width() + 28, size.height() + 4};
}

auto pRadioLabel::setChecked() -> void {
  _parent().lock();
  for(auto& weak : state().group) {
    if(auto item = weak.acquire()) item->state.checked = false;
  }
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget), state().checked = true);
  _parent().unlock();
}

auto pRadioLabel::setGroup(const vector<shared_pointer_weak<mRadioLabel>>& group) -> void {
  if(&_parent() == this) return;
  _parent().lock();
  gtk_radio_button_set_group(
    GTK_RADIO_BUTTON(gtkWidget),
    gtk_radio_button_get_group(GTK_RADIO_BUTTON(_parent().gtkWidget))
  );
  for(auto& weak : state().group) {
    if(auto item = weak.acquire()) {
      if(item->self() && item->checked()) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(item->self()->gtkWidget), true);
        break;
      }
    }
  }
  _parent().unlock();
}

auto pRadioLabel::setText(const string& text) -> void {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
  setFont(self().font(true));  //gtk_button_set_label() recreates label, which destroys currently assigned font
}

auto pRadioLabel::_parent() -> pRadioLabel& {
  if(state().group.size()) {
    if(auto item = state().group.first().acquire()) {
      if(item->self()) return *item->self();
    }
  }
  return *this;
}

}
