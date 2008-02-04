class pVideoGDI;

class VideoGDI : public Video {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  bool lock(uint16_t *&data, unsigned &pitch);
  void unlock();

  void refresh(unsigned width, unsigned height);
  bool init();
  void term();

  VideoGDI();
  ~VideoGDI();

private:
  pVideoGDI &p;
};
