#if defined(Hiro_Desktop)

auto Desktop::size() -> Size {
  return pDesktop::size();
}

auto Desktop::workspace() -> Geometry {
  return pDesktop::workspace();
}

#endif
