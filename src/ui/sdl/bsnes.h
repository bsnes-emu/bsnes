class bJoypad {
public:
bool up, down, left, right;
bool a, b, x, y;
bool l, r, select, start;
  bJoypad();
};

class bSNES : public SNES {
private:
uint32 run_status;
bJoypad joypad1, joypad2;

public:
enum { STOP = 0, RUN };
  void    run();
  void    video_run();
  void    sound_run(uint32 data);

  uint16 *video_lock(uint &data);
  void    video_unlock();

  void    set_status(uint32 new_status);
  uint32  get_status();

//input functions
  void    poll_input(uint8 type);
  bool    get_input_status(uint8 device, uint8 button);

  void    notify(uint32 message, uint32 param1 = 0, uint32 param2 = 0);
  bSNES();
} *bsnes;
