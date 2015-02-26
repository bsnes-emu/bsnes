namespace hiro {

auto pSizable::construct() -> void {
}

auto pSizable::destruct() -> void {
}

auto pSizable::minimumSize() const -> Size {
  return {0, 0};
}

auto pSizable::setGeometry(Geometry geometry) -> void {
}

}
