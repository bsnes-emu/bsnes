class bSNES : public SNES {
public:
uint32 state;
enum {
  RUN, PAUSE
};
  uint32  get_state();
  void    set_state(uint32 new_state);

  void    power();
  void    reset();

  void    scanline();

  void    run();
  void    video_run();
  void    sound_run(uint32 data);

//video functions
  uint16 *video_lock(uint32 &pitch);
  void    video_unlock();

//input functions
  void    poll_input(uint8 type);
  bool    get_input_status(uint8 device, uint8 button);

  bSNES() { state = RUN; }
};

bSNES *bsnes;
