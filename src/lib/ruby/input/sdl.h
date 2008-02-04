class pInputSDL;

class InputSDL : public Input {
public:
  bool key_down(uint16_t key);

  void clear();
  void poll();
  bool init();
  void term();

  InputSDL();
  ~InputSDL();

private:
  pInputSDL &p;
};
