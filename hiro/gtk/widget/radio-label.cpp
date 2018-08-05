#if defined(Hiro_RadioLabel)

namespace hiro {

static auto RadioLabel_activate(GtkToggleButton*, pRadioLabel* p) -> void {
  if(p->groupLocked()) return;
  bool wasChecked = p->state().checked;
  p->self().setChecked();
  if(!wasChecked) p->self().doActivate();
}

auto pRadioLabel::construct() -> void {
  gtkWidget = gtk_radio_button_new_with_label(nullptr, "");
  gtkToggleButton = GTK_TOGGLE_BUTTON(gtkWidget);
  gtkRadioButton = GTK_RADIO_BUTTON(gtkWidget);

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
  auto lock = acquire();
  gtk_toggle_button_set_active(gtkToggleButton, true);
}

auto pRadioLabel::setGroup(sGroup group) -> void {
  maybe<GtkRadioButton*> gtkRadioButton;
  for(auto& weak : group->state.objects) {
    if(auto object = weak.acquire()) {
      if(auto radioLabel = dynamic_cast<mRadioLabel*>(object.data())) {
        if(auto self = radioLabel->self()) {
          auto lock = self->acquire();
          gtk_radio_button_set_group(self->gtkRadioButton, nullptr);
          if(!gtkRadioButton) {
            gtkRadioButton = self->gtkRadioButton;
            gtk_toggle_button_set_active(self->gtkToggleButton, radioLabel->state.checked = true);
          } else {
            gtk_radio_button_set_group(self->gtkRadioButton, gtk_radio_button_get_group(*gtkRadioButton));
            gtk_toggle_button_set_active(self->gtkToggleButton, radioLabel->state.checked = false);
          }
        }
      }
    }
  }
}

auto pRadioLabel::setText(const string& text) -> void {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
  setFont(self().font(true));  //gtk_button_set_label() recreates label, which destroys currently assigned font
}

auto pRadioLabel::groupLocked() const -> bool {
  if(auto group = state().group) {
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto self = object->self()) {
          if(self->locked()) return true;
        }
      }
    }
    return false;
  }
  return locked();
}

}

#endif
