class InputSDL : public Input {
private:
void *window;

public:
  void poll();
  void poll(uint8 device);
  void init();
  void term();

  InputSDL(void *handle = 0);
  ~InputSDL();
};
