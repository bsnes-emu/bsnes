class pInputRaw;

class InputRaw : public Input {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  bool acquire();
  bool unacquire();
  bool acquired();

  bool poll(int16_t *table);
  bool init();
  void term();

  InputRaw();
  ~InputRaw();

private:
  pInputRaw &p;
};
