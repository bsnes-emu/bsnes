#if defined(Hiro_Group)

auto mGroup::allocate() -> pObject* {
  return new pGroup(*this);
}

//

auto mGroup::append(sObject object) -> type& {
  if(auto group = instance.acquire()) {
    state.objects.append(object);
    object->setGroup(group);
  }
  return *this;
}

auto mGroup::object(unsigned position) const -> Object {
  if(position < objectCount()) {
    if(auto object = state.objects[position].acquire()) {
      return object;
    }
  }
  return {};
}

auto mGroup::objectCount() const -> unsigned {
  return state.objects.size();
}

auto mGroup::objects() const -> vector<Object> {
  vector<Object> objects;
  for(auto& weak : state.objects) {
    if(auto object = weak.acquire()) objects.append(object);
  }
  return objects;
}

auto mGroup::remove(sObject object) -> type& {
  object->setGroup();
  for(auto offset : range(state.objects.size())) {
    if(auto shared = state.objects[offset].acquire()) {
      if(object == shared) {
        state.objects.remove(offset);
        break;
      }
    }
  }
  return *this;
}

#endif
