class pInputX;

class InputX : public Input {
public:
  bool key_down(uint16_t key);

  void clear();
  void poll();
  bool init();
  void term();

  InputX();
  ~InputX();

private:
  pInputX &p;
};
