WonderSwanInterface::WonderSwanInterface() {
  information.manufacturer = "Bandai";
  information.name         = "WonderSwan";
  information.overscan     = false;

  media.append({ID::WonderSwan, "WonderSwan", "ws"});
}

//todo: this should be generating grayscale colors
//instead, the PPU is selecting grayscale colors from the color palette

auto WonderSwanInterface::videoColors() -> uint32 {
  return 1 << 12;
}

auto WonderSwanInterface::videoColor(uint32 color) -> uint64 {
  uint b = color.bits(0, 3);
  uint g = color.bits(4, 7);
  uint r = color.bits(8,11);

  uint64_t R = image::normalize(r, 4, 16);
  uint64_t G = image::normalize(g, 4, 16);
  uint64_t B = image::normalize(b, 4, 16);

  if(settings.colorEmulation) {
    R = (r * 26 + g *  4 + b *  2);
    G = (         g * 24 + b *  8);
    B = (r *  6 + g *  4 + b * 22);
    R = image::normalize(min(480, R), 9, 16);
    G = image::normalize(min(480, G), 9, 16);
    B = image::normalize(min(480, B), 9, 16);
  }

  return R << 32 | G << 16 | B << 0;
}

auto WonderSwanInterface::load(uint id) -> bool {
  if(id == ID::WonderSwan) return system.load(this, Model::WonderSwan);
  return false;
}
