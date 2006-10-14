class bSNES : public SNES {
public:
enum { RUN, STOP };
uint32 state;
  uint32  get_state();
  void    set_state(uint32 new_state);

  void    power();
  void    reset();

  void    scanline();

  void    run();
  void    video_run();
  void    sound_run(uint32 data);

//video functions
  uint16 *video_lock(uint &pitch);
  void    video_unlock();

//input functions
  void    poll_input();
  bool    get_input_status(uint8 deviceid, uint8 button);

  bSNES() { state = RUN; }
} *bsnes;
