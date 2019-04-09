#if defined(Hiro_Property)

Property::Property(const string& name, const any& value) {
  state.name = name;
  state.value = value;
}

auto Property::operator==(const Property& source) const -> bool {
  return state.name == source.state.name;
}

auto Property::operator<(const Property& source) const -> bool {
  return state.name < source.state.name;
}

auto Property::name() const -> string {
  return state.name;
}

auto Property::setValue(const any& value) -> type& {
  state.value = value;
  return *this;
}

auto Property::value() const -> any& {
  return state.value;
}

#endif
