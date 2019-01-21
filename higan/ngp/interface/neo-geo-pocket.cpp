auto NeoGeoPocketInterface::information() -> Information {
  Information information;
  information.manufacturer = "SNK";
  information.name         = "Neo Geo Pocket";
  information.extension    = "ngp";
  return information;
}

auto NeoGeoPocketInterface::load() -> bool {
  return system.load(this, System::Model::NeoGeoPocket);
}
