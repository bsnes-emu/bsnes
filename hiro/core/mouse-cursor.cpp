#if defined(Hiro_MouseCursor)

const string MouseCursor::Default = "";
const string MouseCursor::Hand = "{hand}";
const string MouseCursor::HorizontalResize = "{horizontal-resize}";
const string MouseCursor::VerticalResize = "{vertical-resize}";

MouseCursor::MouseCursor(const string& name) {
  setName(name);
}

MouseCursor::operator bool() const {
  return state.name;
}

auto MouseCursor::operator==(const MouseCursor& source) const -> bool {
  return name() == source.name();
}

auto MouseCursor::operator!=(const MouseCursor& source) const -> bool {
  return name() != source.name();
}

auto MouseCursor::name() const -> string {
  return state.name;
}

auto MouseCursor::setName(const string& name) -> type& {
  state.name = name;
  return *this;
}

#endif
