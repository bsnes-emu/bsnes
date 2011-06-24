struct Justifier : Controller {
  uint2 data();
  void latch(bool data);
  Justifier(bool port, bool chained);

//private:
  const bool chained;  //true if the second justifier is attached to the first
  bool latched;
  unsigned counter;

  bool active;
  int x1, x2;
  int y1, y2;
  bool trigger1, trigger2;
  bool start1, start2;
};
