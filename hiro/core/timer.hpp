#if defined(Hiro_Timer)
struct mTimer : mObject {
  Declare(Timer)

  mTimer();

  auto doActivate() const -> void;
  auto interval() const -> uint;
  auto onActivate(const function<void ()>& callback = {}) -> type&;
  auto setInterval(uint interval = 0) -> type&;

//private:
  struct State {
    uint interval = 0;
    function<void ()> onActivate;
  } state;
};
#endif
