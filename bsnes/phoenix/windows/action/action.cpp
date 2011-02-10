Action::Action() {
  OS::os->objects.append(this);
  action = new Action::Data;
}
