#if defined(Hiro_Viewport)
struct mViewport : mWidget {
  Declare(Viewport)

  auto handle() const -> uintptr_t;

//private:
  struct State {
  } state;
};
#endif
