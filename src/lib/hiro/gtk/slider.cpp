void hiro_pslider_change(pSlider *p) {
  if(p->slider_position == p->get_position()) return;
  if(p->self.on_change) p->self.on_change(event_t(event_t::Change, p->slider_position = p->get_position(), &p->self));
}

void pSlider::create(unsigned style, unsigned width, unsigned height, unsigned length) {
  if(length < 1) length = 1;
  if(style & Slider::Vertical) {
    slider = gtk_vscale_new_with_range(0, length - 1, 1);
  } else {
    slider = gtk_hscale_new_with_range(0, length - 1, 1);
  }
  gtk_scale_set_draw_value(GTK_SCALE(slider), FALSE);
  gtk_widget_set_size_request(slider, width, height);
  gtk_widget_show(slider);
  g_signal_connect_swapped(G_OBJECT(slider), "value-changed", G_CALLBACK(hiro_pslider_change), (gpointer)this);
}

unsigned pSlider::get_position() {
  return (unsigned)gtk_range_get_value(GTK_RANGE(slider));
}

void pSlider::set_position(unsigned position) {
  gtk_range_set_value(GTK_RANGE(slider), position);
}

pSlider::pSlider(Slider &self_) : pFormControl(self_), self(self_) {
  slider = 0;
  slider_position = 0;
}

/* internal */

GtkWidget* pSlider::gtk_handle() {
  return slider;
}
