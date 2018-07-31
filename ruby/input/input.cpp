#if defined(INPUT_CARBON)
  #include <ruby/input/carbon.cpp>
#endif

#if defined(INPUT_QUARTZ)
  #include <ruby/input/quartz.cpp>
#endif

#if defined(INPUT_SDL)
  #include <ruby/input/sdl.cpp>
#endif

#if defined(INPUT_UDEV)
  #include <ruby/input/udev.cpp>
#endif

#if defined(INPUT_WINDOWS)
  #include <ruby/input/windows.cpp>
#endif

#if defined(INPUT_XLIB)
  #include <ruby/input/xlib.cpp>
#endif

namespace ruby {

auto Input::setContext(uintptr context) -> bool {
  if(driver->context == context) return true;
  if(!driver->hasContext()) return false;
  if(!driver->setContext(driver->context = context)) return false;
  return true;
}

//

auto Input::acquired() -> bool {
  return driver->acquired();
}

auto Input::acquire() -> bool {
  return driver->acquire();
}

auto Input::release() -> bool {
  return driver->release();
}

auto Input::poll() -> vector<shared_pointer<nall::HID::Device>> {
  return driver->poll();
}

auto Input::rumble(uint64_t id, bool enable) -> bool {
  return driver->rumble(id, enable);
}

//

auto Input::onChange(const function<void (shared_pointer<HID::Device>, uint, uint, int16_t, int16_t)>& onChange) -> void {
  change = onChange;
}

auto Input::doChange(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  if(change) change(device, group, input, oldValue, newValue);
}

//

auto Input::create(string driver) -> bool {
  reset();
  if(!driver) driver = optimalDriver();

  #if defined(INPUT_WINDOWS)
  if(driver == "Windows") self.driver = new InputWindows(*this);
  #endif

  #if defined(INPUT_QUARTZ)
  if(driver == "Quartz") self.driver = new InputQuartz(*this);
  #endif

  #if defined(INPUT_CARBON)
  if(driver == "Carbon") self.driver = new InputCarbon(*this);
  #endif

  #if defined(INPUT_UDEV)
  if(driver == "udev") self.driver = new InputUdev(*this);
  #endif

  #if defined(INPUT_SDL)
  if(driver == "SDL") self.driver = new InputSDL(*this);
  #endif

  #if defined(INPUT_XLIB)
  if(driver == "Xlib") self.driver = new InputXlib(*this);
  #endif

  if(!self.driver) self.driver = new InputDriver(*this);

  return self.driver->create();
}

auto Input::hasDrivers() -> vector<string> {
  return {

  #if defined(INPUT_WINDOWS)
  "Windows",
  #endif

  #if defined(INPUT_QUARTZ)
  "Quartz",
  #endif

  #if defined(INPUT_CARBON)
  "Carbon",
  #endif

  #if defined(INPUT_UDEV)
  "udev",
  #endif

  #if defined(INPUT_SDL)
  "SDL",
  #endif

  #if defined(INPUT_XLIB)
  "Xlib",
  #endif

  "None"};
}

auto Input::optimalDriver() -> string {
  #if defined(INPUT_WINDOWS)
  return "Windows";
  #elif defined(INPUT_QUARTZ)
  return "Quartz";
  #elif defined(INPUT_CARBON)
  return "Carbon";
  #elif defined(INPUT_UDEV)
  return "udev";
  #elif defined(INPUT_SDL)
  return "SDL";
  #elif defined(INPUT_XLIB)
  return "Xlib";
  #else
  return "None";
  #endif
}

auto Input::safestDriver() -> string {
  #if defined(INPUT_WINDOWS)
  return "Windows";
  #elif defined(INPUT_QUARTZ)
  return "Quartz";
  #elif defined(INPUT_CARBON)
  return "Carbon";
  #elif defined(INPUT_UDEV)
  return "udev";
  #elif defined(INPUT_SDL)
  return "SDL";
  #elif defined(INPUT_XLIB)
  return "Xlib";
  #else
  return "none";
  #endif
}

}
