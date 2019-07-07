auto GameBoyInterface::information() -> Information {
  Information information;
  information.manufacturer = "Nintendo";
  information.name         = "Game Boy";
  information.extension    = "gb";
  return information;
}

auto GameBoyInterface::color(uint32 color) -> uint64 {
  uint64 L = image::normalize(3 - color, 2, 16);
  return L << 32 | L << 16 | L << 0;
}

auto GameBoyInterface::load() -> bool {
  return system.load(this, System::Model::GameBoy);
}
