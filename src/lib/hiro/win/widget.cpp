void pWidget::show(bool state) {
}

void pWidget::hide() {
}

bool pWidget::visible() {
  return true;
}

uintptr_t pWidget::handle() {
  return 0;
}

pWidget::pWidget(Widget &self_) : self(self_) {
  instance = instance_counter++;
  phiro().widget_list.add(&self);
}

pWidget::~pWidget() {
}

/* internal */

//100 is the standard start index for control IDs in the Windows API
//avoids duplicate IDs when they are not explicitly set (and are thus 0)
unsigned pWidget::instance_counter = 100;
