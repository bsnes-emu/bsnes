auto WonderSwanInterface::information() -> Information {
  Information information;
  information.manufacturer = "Bandai";
  information.name         = "WonderSwan";
  information.extension    = "ws";
  return information;
}

auto WonderSwanInterface::load() -> bool {
  return system.load(this, System::Model::WonderSwan);
}
