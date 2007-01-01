class bSNES {
public:
enum { RUN, STOP };
uint state;
  uint get_state();
  void set_state(uint new_state);
  void run();
  void video_run();

  bSNES() { state = RUN; }
} bsnes;
