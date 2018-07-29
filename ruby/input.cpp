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
  _context = context;
  return true;
}

auto Input::onChange(const function<void (shared_pointer<HID::Device>, uint, uint, int16_t, int16_t)>& onChange) -> void {
  _onChange = onChange;
}

auto Input::doChange(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  if(_onChange) _onChange(device, group, input, oldValue, newValue);
}

//protected functions

//static functions

auto Input::create(string driver) -> Input* {
  Input* input = nullptr;
  if(!driver) driver = optimalDriver();

  #if defined(INPUT_WINDOWS)
  if(driver == "Windows") input = new InputWindows;
  #endif

  #if defined(INPUT_QUARTZ)
  if(driver == "Quartz") input = new InputQuartz;
  #endif

  #if defined(INPUT_CARBON)
  if(driver == "Carbon") input = new InputCarbon;
  #endif

  #if defined(INPUT_UDEV)
  if(driver == "udev") input = new InputUdev;
  #endif

  #if defined(INPUT_SDL)
  if(driver == "SDL") input = new InputSDL;
  #endif

  #if defined(INPUT_XLIB)
  if(driver == "Xlib") input = new InputXlib;
  #endif

  if(!input) input = new Input;

  input->_context = input->context();

  return input;
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

auto Input::availableDrivers() -> vector<string> {
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

}
