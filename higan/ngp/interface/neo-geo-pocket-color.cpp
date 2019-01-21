auto NeoGeoPocketColorInterface::information() -> Information {
  Information information;
  information.manufacturer = "SNK";
  information.name         = "Neo Geo Pocket Color";
  information.extension    = "ngpc";
  return information;
}

auto NeoGeoPocketColorInterface::load() -> bool {
  return system.load(this, System::Model::NeoGeoPocketColor);
}
