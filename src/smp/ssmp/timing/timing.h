template<uint8 cycle_frequency> class sSMPTimer {
public:
uint8 target;
uint8 stage1_ticks, stage2_ticks, stage3_ticks;
bool  enabled;

  void tick() {
  //stage 1 increment
    stage1_ticks++;
    if(stage1_ticks < cycle_frequency)return;

    stage1_ticks -= cycle_frequency;
    if(enabled == false)return;

  //stage 2 increment
    stage2_ticks++;

    if(stage2_ticks != target)return;

  //stage 3 increment
    stage2_ticks = 0;
    stage3_ticks++;
    stage3_ticks &= 15;
  }
};

sSMPTimer<128> t0;
sSMPTimer<128> t1;
sSMPTimer< 16> t2;

  void   add_clocks(uint clocks);
  uint32 clocks_executed();
