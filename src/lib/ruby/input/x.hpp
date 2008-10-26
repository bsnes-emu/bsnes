class pInputX;

class InputX : public Input {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  bool poll(int16_t *table);
  bool init();
  void term();

  InputX();
  ~InputX();

private:
  pInputX &p;
};
