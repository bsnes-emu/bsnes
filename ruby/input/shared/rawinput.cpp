#ifndef RUBY_INPUT_SHARED_RAWINPUT
#define RUBY_INPUT_SHARED_RAWINPUT

namespace ruby {

LRESULT CALLBACK RawInputWindowProc(HWND, UINT, WPARAM, LPARAM);

struct RawInput {
  HANDLE mutex;
  HWND hwnd;
  bool ready = false;
  bool initialized = false;
  function<void (RAWINPUT*)> updateKeyboard;
  function<void (RAWINPUT*)> updateMouse;

  struct Device {
    HANDLE handle;
    string path;
    enum class Type : unsigned { Keyboard, Mouse, Joypad } type;
    uint16_t vendorID = 0;
    uint16_t productID = 0;
    bool isXInputDevice = false;
  };
  vector<Device> devices;

  optional<Device&> find(uint16_t vendorID, uint16_t productID) {
    for(auto& device : devices) {
      if(device.vendorID == vendorID && device.productID == productID) return {true, device};
    }
    return false;
  }

  void scanDevices() {
    devices.reset();

    unsigned deviceCount = 0;
    GetRawInputDeviceList(NULL, &deviceCount, sizeof(RAWINPUTDEVICELIST));
    RAWINPUTDEVICELIST* list = new RAWINPUTDEVICELIST[deviceCount];
    GetRawInputDeviceList(list, &deviceCount, sizeof(RAWINPUTDEVICELIST));

    for(unsigned n = 0; n < deviceCount; n++) {
      wchar_t path[4096];
      unsigned size = sizeof(path) - 1;
      GetRawInputDeviceInfo(list[n].hDevice, RIDI_DEVICENAME, &path, &size);

      RID_DEVICE_INFO info;
      info.cbSize = size = sizeof(RID_DEVICE_INFO);
      GetRawInputDeviceInfo(list[n].hDevice, RIDI_DEVICEINFO, &info, &size);

      Device device;
      device.path = (const char*)utf8_t(path);
      device.handle = list[n].hDevice;

      if(info.dwType == RIM_TYPEKEYBOARD) {
        device.type = Device::Type::Keyboard;
        device.vendorID = 0;
        device.productID = 1;
      }

      if(info.dwType == RIM_TYPEMOUSE) {
        device.type = Device::Type::Mouse;
        device.vendorID = 0;
        device.productID = 2;
      }

      if(info.dwType == RIM_TYPEHID) {
        //verify that this is a joypad device
        if(info.hid.usUsagePage != 1 || (info.hid.usUsage != 4 && info.hid.usUsage != 5)) continue;

        device.type = Device::Type::Joypad;
        device.vendorID = info.hid.dwVendorId;
        device.productID = info.hid.dwProductId;
        if(device.path.find("IG_")) device.isXInputDevice = true;  //"IG_" is only found inside XInput device paths
      }

      devices.append(device);
    }

    delete[] list;
  }

  LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if(msg != WM_INPUT) return DefWindowProc(hwnd, msg, wparam, lparam);

    unsigned size = 0;
    GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
    RAWINPUT* input = new RAWINPUT[size];
    GetRawInputData((HRAWINPUT)lparam, RID_INPUT, input, &size, sizeof(RAWINPUTHEADER));
    WaitForSingleObject(mutex, INFINITE);

    if(input->header.dwType == RIM_TYPEKEYBOARD) {
      if(updateKeyboard) updateKeyboard(input);
    }

    if(input->header.dwType == RIM_TYPEMOUSE) {
      if(updateMouse) updateMouse(input);
    }

    ReleaseMutex(mutex);
    LRESULT result = DefRawInputProc(&input, size, sizeof(RAWINPUTHEADER));
    delete[] input;
    return result;
  }

  void main() {
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hInstance = GetModuleHandle(0);
    wc.lpfnWndProc = RawInputWindowProc;
    wc.lpszClassName = L"RawInputClass";
    wc.lpszMenuName = 0;
    wc.style = CS_VREDRAW | CS_HREDRAW;
    RegisterClass(&wc);

    hwnd = CreateWindow(L"RawInputClass", L"RawInputClass", WS_POPUP, 0, 0, 64, 64, 0, 0, GetModuleHandle(0), 0);

    scanDevices();

    RAWINPUTDEVICE device[2];
    //capture all keyboard input
    device[0].usUsagePage = 1;
    device[0].usUsage = 6;
    device[0].dwFlags = RIDEV_INPUTSINK;
    device[0].hwndTarget = hwnd;
    //capture all mouse input
    device[1].usUsagePage = 1;
    device[1].usUsage = 2;
    device[1].dwFlags = RIDEV_INPUTSINK;
    device[1].hwndTarget = hwnd;
    RegisterRawInputDevices(device, 2, sizeof(RAWINPUTDEVICE));

    WaitForSingleObject(mutex, INFINITE);
    ready = true;
    ReleaseMutex(mutex);

    while(true) {
      MSG msg;
      GetMessage(&msg, hwnd, 0, 0);
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
};

static RawInput rawinput;

DWORD WINAPI RawInputThreadProc(void*) {
  rawinput.main();
  return 0;
}

LRESULT CALLBACK RawInputWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return rawinput.windowProc(hwnd, msg, wparam, lparam);
}

}

#endif
