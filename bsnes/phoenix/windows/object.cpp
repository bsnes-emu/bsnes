vector<pObject*> pObject::objects;

pObject::pObject(Object &object) : object(object) {
  static unsigned uniqueId = 100;
  objects.append(this);
  id = uniqueId++;
  locked = false;
}

pObject* pObject::find(unsigned id) {
  for(auto &item : objects) if(item->id == id) return item;
  return 0;
}
