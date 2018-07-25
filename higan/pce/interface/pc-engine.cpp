auto PCEngineInterface::information() -> Information {
  Information information;
  information.manufacturer = "NEC";
  information.name         = "PC Engine";
  information.extension    = "pce";
  return information;
}

auto PCEngineInterface::load() -> bool {
  return system.load(this, System::Model::PCEngine);
}
