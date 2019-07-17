#if defined(Hiro_Geometry)

Geometry::Geometry() {
  setGeometry(0, 0, 0, 0);
}

Geometry::Geometry(Position position, Size size) {
  setGeometry(position, size);
}

Geometry::Geometry(float x, float y, float width, float height) {
  setGeometry(x, y, width, height);
}

Geometry::operator bool() const {
  return state.x || state.y || state.width || state.height;
}

auto Geometry::operator==(const Geometry& source) const -> bool {
  return x() == source.x() && y() == source.y() && width() == source.width() && height() == source.height();
}

auto Geometry::operator!=(const Geometry& source) const -> bool {
  return !operator==(source);
}

auto Geometry::height() const -> float {
  return state.height;
}

auto Geometry::position() const -> Position {
  return {state.x, state.y};
}

auto Geometry::reset() -> type& {
  return setGeometry(0, 0, 0, 0);
}

auto Geometry::setHeight(float height) -> type& {
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

auto Geometry::setGeometry(float x, float y, float width, float height) -> type& {
  state.x = x;
  state.y = y;
  state.width = width;
  state.height = height;
  return *this;
}

auto Geometry::setPosition(Position position) -> type& {
  return setPosition(position.x(), position.y());
}

auto Geometry::setPosition(float x, float y) -> type& {
  state.x = x;
  state.y = y;
  return *this;
}

auto Geometry::setSize(Size size) -> type& {
  return setSize(size.width(), size.height());
}

auto Geometry::setSize(float width, float height) -> type& {
  state.width = width;
  state.height = height;
  return *this;
}

auto Geometry::setWidth(float width) -> type& {
  state.width = width;
  return *this;
}

auto Geometry::setX(float x) -> type& {
  state.x = x;
  return *this;
}

auto Geometry::setY(float y) -> type& {
  state.y = y;
  return *this;
}

auto Geometry::size() const -> Size {
  return {state.width, state.height};
}

auto Geometry::width() const -> float {
  return state.width;
}

auto Geometry::x() const -> float {
  return state.x;
}

auto Geometry::y() const -> float {
  return state.y;
}

#endif
