struct Multitap : Controller {
  Multitap(bool port);

  auto data() -> uint2;
  auto latch(bool data) -> void;

private:
  bool latched;
  uint counter1;
  uint counter2;
};
