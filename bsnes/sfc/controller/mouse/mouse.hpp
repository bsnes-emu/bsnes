struct Mouse : Controller {
  uint2 data();
  void latch(bool data);
  Mouse(bool port);

private:
  bool latched;
  unsigned counter;

  struct MouseInput {
    signed x;  //x-coordinate
    signed y;  //y-coordinate
    bool dx;   //x-direction
    bool dy;   //y-direction
    bool l;    //left button
    bool r;    //right button
  } input;
};
