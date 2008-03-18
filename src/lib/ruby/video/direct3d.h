class pVideoD3D;

class VideoD3D : public Video {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  bool lock(uint32_t *&data, unsigned &pitch);
  void unlock();

  void clear();
  void refresh(unsigned width, unsigned height);
  bool init();
  void term();

  VideoD3D();
  ~VideoD3D();

private:
  pVideoD3D &p;
};
