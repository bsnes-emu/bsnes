auto GameBoyColorInterface::information() -> Information {
  Information information;
  information.manufacturer = "Nintendo";
  information.name         = "Game Boy Color";
  information.extension    = "gbc";
  return information;
}

auto GameBoyColorInterface::color(uint32 color) -> uint64 {
  uint r = bits(color, 0- 4);
  uint g = bits(color, 5- 9);
  uint b = bits(color,10-14);

  uint64 R = image::normalize(r, 5, 16);
  uint64 G = image::normalize(g, 5, 16);
  uint64 B = image::normalize(b, 5, 16);

  return R << 32 | G << 16 | B << 0;
}

auto GameBoyColorInterface::load() -> bool {
  return system.load(this, System::Model::GameBoyColor);
}
