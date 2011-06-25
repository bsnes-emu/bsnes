struct Justifier : Controller {
  void enter();
  uint2 data();
  void latch(bool data);
  Justifier(bool port, bool chained);

//private:
  const bool chained;  //true if the second justifier is attached to the first
  bool latched;
  unsigned counter;

  bool active;
  signed x1, x2;
  signed y1, y2;
  signed target;
  bool trigger1, trigger2;
  bool start1, start2;
};
