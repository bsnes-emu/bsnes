Geometry::Geometry() {
  setGeometry(0, 0, 0, 0);
}

Geometry::Geometry(Position position, Size size) {
  setGeometry(position, size);
}

Geometry::Geometry(signed x, signed y, signed width, signed height) {
  setGeometry(x, y, width, height);
}

Geometry::Geometry(const string& text) {
  lstring part = text.split(",").strip();
  state.x = integer(part(0));
  state.y = integer(part(1));
  state.width = integer(part(2));
  state.height = integer(part(3));
}

auto Geometry::operator==(const Geometry& source) const -> bool {
  return x() == source.x() && y() == source.y() && width() == source.width() && height() == source.height();
}

auto Geometry::operator!=(const Geometry& source) const -> bool {
  return !operator==(source);
}

auto Geometry::height() const -> signed {
  return state.height;
}

auto Geometry::position() const -> Position {
  return {state.x, state.y};
}

auto Geometry::setHeight(signed height) -> type& {
  state.height = height;
  return *this;
}

auto Geometry::setGeometry(Geometry geometry) -> type& {
  return setGeometry(geometry.x(), geometry.y(), geometry.width(), geometry.height());
}

auto Geometry::setGeometry(Position position, Size size) -> type& {
  setGeometry(position.x(), position.y(), size.width(), size.height());
  return *this;
}

auto Geometry::setGeometry(signed x, signed y, signed width, signed height) -> type& {
  state.x = x;
  state.y = y;
  state.width = width;
  state.height = height;
  return *this;
}

auto Geometry::setPosition(Position position) -> type& {
  return setPosition(position.x(), position.y());
}

auto Geometry::setPosition(signed x, signed y) -> type& {
  state.x = x;
  state.y = y;
  return *this;
}

auto Geometry::setSize(Size size) -> type& {
  return setSize(size.width(), size.height());
}

auto Geometry::setSize(signed width, signed height) -> type& {
  state.width = width;
  state.height = height;
  return *this;
}

auto Geometry::setWidth(signed width) -> type& {
  state.width = width;
  return *this;
}

auto Geometry::setX(signed x) -> type& {
  state.x = x;
  return *this;
}

auto Geometry::setY(signed y) -> type& {
  state.y = y;
  return *this;
}

auto Geometry::size() const -> Size {
  return {state.width, state.height};
}

auto Geometry::text() const -> string {
  return {state.x, ",", state.y, ",", state.width, ",", state.height};
}

auto Geometry::width() const -> signed {
  return state.width;
}

auto Geometry::x() const -> signed {
  return state.x;
}

auto Geometry::y() const -> signed {
  return state.y;
}
