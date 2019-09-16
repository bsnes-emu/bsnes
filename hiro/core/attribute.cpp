#if defined(Hiro_Attribute)

Attribute::Attribute(const string& name, const any& value) {
  state.name = name;
  state.value = value;
}

auto Attribute::operator==(const Attribute& source) const -> bool {
  return state.name == source.state.name;
}

auto Attribute::operator<(const Attribute& source) const -> bool {
  return state.name < source.state.name;
}

auto Attribute::name() const -> string {
  return state.name;
}

auto Attribute::setValue(const any& value) -> type& {
  state.value = value;
  return *this;
}

auto Attribute::value() const -> any& {
  return state.value;
}

#endif
