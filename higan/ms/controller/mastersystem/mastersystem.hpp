struct MasterSystemControls : Controller {
  enum : uint {
    Reset,
  };

  MasterSystemControls(uint port);

  auto readData() -> uint8 override;
};
