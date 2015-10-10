struct Mouse : Controller {
  Mouse(bool port);

  auto data() -> uint2;
  auto latch(bool data) -> void;

private:
  bool latched;
  unsigned counter;

  unsigned speed;  //0 = slow, 1 = normal, 2 = fast
  signed x;        //x-coordinate
  signed y;        //y-coordinate
  bool dx;         //x-direction
  bool dy;         //y-direction
  bool l;          //left button
  bool r;          //right button
};
