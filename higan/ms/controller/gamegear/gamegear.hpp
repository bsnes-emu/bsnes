struct GameGearControls : Controller {
  enum : uint {
    Up, Down, Left, Right, One, Two, Start,
  };

  GameGearControls(uint port);

  auto readData() -> uint8 override;
};
