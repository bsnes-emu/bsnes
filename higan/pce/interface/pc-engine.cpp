PCEngineInterface::PCEngineInterface() {
  information.manufacturer = "NEC";
  information.name         = "PC Engine";

  media.append({ID::PCEngine, "PC Engine", "pce"});
}

auto PCEngineInterface::load(uint id) -> bool {
  if(id == ID::PCEngine) return system.load(this, id);
  return false;
}
