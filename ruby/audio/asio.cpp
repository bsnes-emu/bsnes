#include "asio.hpp"

struct AudioASIO : Audio {
  ~AudioASIO() { term(); }

  struct Settings {
    HWND handle = nullptr;
    bool synchronize = true;
    uint frequency = 48000;
  } settings;

  struct Driver {
    string name;
    string classID;
  };
  vector<Driver> drivers;

  Driver driver;
  IASIO* device = nullptr;

  auto cap(const string& name) -> bool {
    if(name == Audio::Handle) return true;
    if(name == Audio::Synchronize) return true;
    if(name == Audio::Frequency) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Audio::Handle) return (uintptr)settings.handle;
    if(name == Audio::Synchronize) return settings.synchronize;
    if(name == Audio::Frequency) return settings.frequency;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Audio::Handle && value.is<uintptr>()) {
      settings.handle = (HWND)value.get<uintptr>();
      return true;
    }

    if(name == Audio::Synchronize && value.is<bool>()) {
      settings.synchronize = value.get<bool>();
      return true;
    }

    if(name == Audio::Frequency && value.is<uint>()) {
      settings.frequency = value.get<uint>();
      return true;
    }

    return false;
  }

  auto sample(int16_t left, int16_t right) -> void {
  }

  auto clear() -> void {
  }

  auto init() -> bool {
    //enumerate available ASIO drivers from the registry
    for(auto candidate : registry::contents("HKLM\\SOFTWARE\\ASIO\\")) {
      if(auto classID = registry::read({"HKLM\\SOFTWARE\\ASIO\\", candidate, "CLSID"})) {
        drivers.append({candidate.trimRight("\\", 1L), classID});
      }
    }
    if(!drivers) return false;

    //default to first driver for now
    driver = drivers[0];

    CLSID classID;
    if(CLSIDFromString((LPOLESTR)utf16_t(driver.classID), (LPCLSID)&classID) != S_OK) return false;
    if(CoCreateInstance(classID, 0, CLSCTX_INPROC_SERVER, classID, (void**)&device) != S_OK) return false;

    if(!device->init((void*)settings.handle)) return false;

    //temporary debugging information
    char driverName[4096] = {0};
    device->getDriverName(driverName);
    print("Driver: ", driverName, "\n");
    print("Version: ", device->getDriverVersion(), "\n");
    print("---\n");

    return true;
  }

  auto term() -> void {
  }
};
