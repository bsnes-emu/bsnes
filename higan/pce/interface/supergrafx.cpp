auto SuperGrafxInterface::information() -> Information {
  Information information;
  information.manufacturer = "NEC";
  information.name         = "SuperGrafx";
  information.extension    = "sg";
  return information;
}

auto SuperGrafxInterface::load() -> bool {
  return system.load(this, System::Model::SuperGrafx);
}
