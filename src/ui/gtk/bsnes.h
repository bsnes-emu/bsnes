class bSNES : public SNES {
private:
uint32 run_status;

public:
enum { STOP = 0, RUN };
  void    run();
  void    video_run();
  void    sound_run(uint32 data);

  uint16 *video_lock(uint &pitch);
  void    video_unlock();

  void    set_status(uint32 status);
  uint32  get_status();

  void    poll_input(uint8 type);
  bool    get_input_status(uint8 device, uint8 button);

  void    notify(uint32 message, uint32 param1, uint32 param2);
  bSNES();
} *bsnes;
