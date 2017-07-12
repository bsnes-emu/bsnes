GameBoyColorInterface::GameBoyColorInterface() {
  information.manufacturer = "Nintendo";
  information.name         = "Game Boy Color";
  information.overscan     = false;

  media.append({ID::GameBoyColor, "Game Boy Color", "gbc"});
}

auto GameBoyColorInterface::videoColors() -> uint32 {
  return 1 << 15;
}

auto GameBoyColorInterface::videoColor(uint32 color) -> uint64 {
  uint r = color.bits( 0, 4);
  uint g = color.bits( 5, 9);
  uint b = color.bits(10,14);

  uint64_t R = image::normalize(r, 5, 16);
  uint64_t G = image::normalize(g, 5, 16);
  uint64_t B = image::normalize(b, 5, 16);

  if(settings.colorEmulation) {
    R = (r * 26 + g *  4 + b *  2);
    G = (         g * 24 + b *  8);
    B = (r *  6 + g *  4 + b * 22);
    R = image::normalize(min(960, R), 10, 16);
    G = image::normalize(min(960, G), 10, 16);
    B = image::normalize(min(960, B), 10, 16);
  }

  return R << 32 | G << 16 | B << 0;
}

auto GameBoyColorInterface::load(uint id) -> bool {
  if(id == ID::GameBoyColor) return system.load(this, System::Model::GameBoyColor);
  return false;
}
