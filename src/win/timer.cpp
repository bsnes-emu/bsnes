class fpstimer {
private:
SYSTEMTIME st;
int second, ticks;
public:
  void start();
  void tick();
  int  get_ticks();
  void reset();
  bool second_passed();

  fpstimer();
};

void fpstimer::start() {
  GetSystemTime(&st);
  second = st.wSecond;
}

void fpstimer::tick() {
  ticks++;
}

int fpstimer::get_ticks() {
  return ticks;
}

void fpstimer::reset() {
  ticks = 0;
}

bool fpstimer::second_passed() {
  GetSystemTime(&st);
  if(st.wSecond != second) {
    second = st.wSecond;
    return true;
  }
  return false;
}

fpstimer::fpstimer() {
  second = ticks = 0;
}
