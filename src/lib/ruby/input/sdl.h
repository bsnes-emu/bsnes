class pInputSDL;

class InputSDL : public Input {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

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
