struct InputUiGeneral : public InputGroup {
  InputObject loadCartridge;
  InputObject pauseEmulation;
  InputObject resetSystem;
  InputObject powerCycleSystem;
  InputObject lowerSpeed;
  InputObject raiseSpeed;
  InputObject toggleCheatSystem;
  InputObject toggleFullscreen;
  InputObject toggleMenu;
  InputObject toggleStatus;
  InputObject exitEmulator;

  InputUiGeneral();
};

struct InputGroupPool : public array<InputGroup*> {
  void attach(InputGroup &group);
  void bind();
  void clear();
  void poll(const int16_t *table);
  InputGroupPool();

private:
  array<InputGroup*> &list;
};

struct InputUiPool : public InputGroupPool {
  InputUiPool();
};

extern InputUiGeneral inputUiGeneral;
extern InputUiPool inputUiPool;
