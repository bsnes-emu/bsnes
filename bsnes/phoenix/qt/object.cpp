void Object::unused() {
}

Object::Object() {
  object = new Object::Data(*this);
}
