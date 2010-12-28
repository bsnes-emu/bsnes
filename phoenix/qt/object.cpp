void Object::unused() {
}

Object::Object() {
  OS::initialize();
  object = new Object::Data(*this);
}
