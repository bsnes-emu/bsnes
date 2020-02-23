#pragma once

#include <IOKit/hid/IOHIDLib.h>

auto deviceMatchingCallback(void* context, IOReturn result, void* sender, IOHIDDeviceRef device) -> void;
auto deviceRemovalCallback(void* context, IOReturn result, void* sender, IOHIDDeviceRef device) -> void;

struct InputJoypadIOKit {
  Input& input;
  InputJoypadIOKit(Input& input) : input(input) {}

  struct Joypad {
    auto appendElements(CFArrayRef elements) -> void {
      for(uint n : range(CFArrayGetCount(elements))) {
        IOHIDElementRef element = (IOHIDElementRef)CFArrayGetValueAtIndex(elements, n);
        IOHIDElementType type = IOHIDElementGetType(element);
        uint32_t usage = IOHIDElementGetUsage(element);
        switch(type) {
        case kIOHIDElementTypeInput_Button:
          appendButton(element);
          break;
        case kIOHIDElementTypeInput_Axis:
        case kIOHIDElementTypeInput_Misc:
          if(usage == kHIDUsage_Sim_Accelerator || usage == kHIDUsage_Sim_Brake
          || usage == kHIDUsage_Sim_Rudder      || usage == kHIDUsage_Sim_Throttle
          || usage == kHIDUsage_GD_X  || usage == kHIDUsage_GD_Y  || usage == kHIDUsage_GD_Z
          || usage == kHIDUsage_GD_Rx || usage == kHIDUsage_GD_Ry || usage == kHIDUsage_GD_Rz
          || usage == kHIDUsage_GD_Slider || usage == kHIDUsage_GD_Dial || usage == kHIDUsage_GD_Wheel
          ) appendAxis(element);
          if(usage == kHIDUsage_GD_DPadUp   || usage == kHIDUsage_GD_DPadDown
          || usage == kHIDUsage_GD_DPadLeft || usage == kHIDUsage_GD_DPadRight
          || usage == kHIDUsage_GD_Start    || usage == kHIDUsage_GD_Select
          || usage == kHIDUsage_GD_SystemMainMenu
          ) appendButton(element);
          if(usage == kHIDUsage_GD_Hatswitch
          ) appendHat(element);
          break;
        case kIOHIDElementTypeCollection:
          if(CFArrayRef children = IOHIDElementGetChildren(element)) appendElements(children);
          break;
        }
      }
    }

    auto appendAxis(IOHIDElementRef element) -> void {
      IOHIDElementCookie cookie = IOHIDElementGetCookie(element);
      if(auto duplicate = axes.find([cookie](auto axis) { return IOHIDElementGetCookie(axis) == cookie; })) {
        return;
      }

      int min = IOHIDElementGetLogicalMin(element);
      int max = IOHIDElementGetLogicalMax(element);
      int range = max - min;
      if(range == 0) return;

      hid->axes().append(axes.size());
      axes.append(element);
    }

    auto appendHat(IOHIDElementRef element) -> void {
      IOHIDElementCookie cookie = IOHIDElementGetCookie(element);
      if(auto duplicate = hats.find([cookie](auto hat) { return IOHIDElementGetCookie(hat) == cookie; })) {
        return;
      }

      uint n = hats.size() * 2;
      hid->hats().append(n + 0);
      hid->hats().append(n + 1);
      hats.append(element);
    }

    auto appendButton(IOHIDElementRef element) -> void {
      IOHIDElementCookie cookie = IOHIDElementGetCookie(element);
      if(auto duplicate = buttons.find([cookie](auto button) { return IOHIDElementGetCookie(button) == cookie; })) {
        return;
      }

      hid->buttons().append(buttons.size());
      buttons.append(element);
    }

    shared_pointer<HID::Joypad> hid{new HID::Joypad};

    IOHIDDeviceRef device = nullptr;
    vector<IOHIDElementRef> axes;
    vector<IOHIDElementRef> hats;
    vector<IOHIDElementRef> buttons;
  };
  vector<Joypad> joypads;
  IOHIDManagerRef manager = nullptr;

  enum : int { Center = 0, Up = -1, Down = +1, Left = -1, Right = +1 };

  auto assign(shared_pointer<HID::Joypad> hid, uint groupID, uint inputID, int16_t value) -> void {
    auto& group = hid->group(groupID);
    if(group.input(inputID).value() == value) return;
    input.doChange(hid, groupID, inputID, group.input(inputID).value(), value);
    group.input(inputID).setValue(value);
  }

  auto poll(vector<shared_pointer<HID::Device>>& devices) -> void {
    for(auto& jp : joypads) {
      IOHIDDeviceRef device = jp.device;

      for(uint n : range(jp.axes.size())) {
        int value = 0;
        IOHIDValueRef valueRef;
        if(IOHIDDeviceGetValue(device, jp.axes[n], &valueRef) == kIOReturnSuccess) {
          int min = IOHIDElementGetLogicalMin(jp.axes[n]);
          int max = IOHIDElementGetLogicalMax(jp.axes[n]);
          int range = max - min;
          value = (IOHIDValueGetIntegerValue(valueRef) - min) * 65535LL / range - 32767;
        }
        assign(jp.hid, HID::Joypad::GroupID::Axis, n, sclamp<16>(value));
      }

      for(uint n : range(jp.hats.size())) {
        int x = Center;
        int y = Center;
        IOHIDValueRef valueRef;
        if(IOHIDDeviceGetValue(device, jp.hats[n], &valueRef) == kIOReturnSuccess) {
          int position = IOHIDValueGetIntegerValue(valueRef);
          int min = IOHIDElementGetLogicalMin(jp.hats[n]);
          int max = IOHIDElementGetLogicalMax(jp.hats[n]);
          if(position >= min && position <= max) {
            position -= min;
            int range = max - min + 1;
            if(range == 4) {
              position *= 2;
            }
            if(range == 8) {
              switch(position) {
              case 0: x = Up;     y = Center; break;
              case 1: x = Up;     y = Right;  break;
              case 2: x = Center; y = Right;  break;
              case 3: x = Down;   y = Right;  break;
              case 4: x = Down;   y = Center; break;
              case 5: x = Down;   y = Left;   break;
              case 6: x = Center; y = Left;   break;
              case 7: x = Up;     y = Left;   break;
              }
            }
          }
        }
        assign(jp.hid, HID::Joypad::GroupID::Hat, n * 2 + 0, x * 32767);
        assign(jp.hid, HID::Joypad::GroupID::Hat, n * 2 + 1, y * 32767);
      }

      for(uint n : range(jp.buttons.size())) {
        int value = 0;
        IOHIDValueRef valueRef;
        if(IOHIDDeviceGetValue(device, jp.buttons[n], &valueRef) == kIOReturnSuccess) {
          value = IOHIDValueGetIntegerValue(valueRef);
        }
        assign(jp.hid, HID::Joypad::GroupID::Button, n, (bool)value);
      }

      devices.append(jp.hid);
    }
  }

  auto rumble(uint64_t id, bool enable) -> bool {
    //todo
    return false;
  }

  auto initialize() -> bool {
    manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    if(!manager) return false;

    CFArrayRef matcher = createMatcher();
    if(!matcher) {
      releaseManager();
      return false;
    }

    IOHIDManagerSetDeviceMatchingMultiple(manager, matcher);
    IOHIDManagerRegisterDeviceMatchingCallback(manager, deviceMatchingCallback, this);
    IOHIDManagerRegisterDeviceRemovalCallback(manager, deviceRemovalCallback, this);
    IOHIDManagerScheduleWithRunLoop(manager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);

    if(IOHIDManagerOpen(manager, kIOHIDOptionsTypeNone) != kIOReturnSuccess) {
      releaseManager();
      return false;
    }

    return true;
  }

  auto terminate() -> void {
    if(!manager) return;
    IOHIDManagerClose(manager, kIOHIDOptionsTypeNone);
    releaseManager();
  }

  auto appendJoypad(IOHIDDeviceRef device) -> void {
    Joypad jp;
    jp.device = device;

    int32_t vendorID, productID;
    CFNumberGetValue((CFNumberRef)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDVendorIDKey)), kCFNumberSInt32Type, &vendorID);
    CFNumberGetValue((CFNumberRef)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductIDKey)), kCFNumberSInt32Type, &productID);
    jp.hid->setVendorID(vendorID);
    jp.hid->setProductID(productID);

    CFArrayRef elements = IOHIDDeviceCopyMatchingElements(device, nullptr, kIOHIDOptionsTypeNone);
    if(elements) {
      jp.appendElements(elements);
      CFRelease(elements);
      joypads.append(jp);
    }
  }

  auto removeJoypad(IOHIDDeviceRef device) -> void {
    for(uint n : range(joypads.size())) {
      if(joypads[n].device == device) {
        joypads.remove(n);
        return;
      }
    }
  }

private:
  auto releaseManager() -> void {
    CFRelease(manager);
    manager = nullptr;
  }

  auto createMatcher() -> CFArrayRef {
    CFDictionaryRef dict1 = createMatcherCriteria(kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick);
    if(!dict1) return nullptr;
    CFDictionaryRef dict2 = createMatcherCriteria(kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad);
    if(!dict2) return CFRelease(dict1), nullptr;
    CFDictionaryRef dict3 = createMatcherCriteria(kHIDPage_GenericDesktop, kHIDUsage_GD_MultiAxisController);
    if(!dict3) return CFRelease(dict1), CFRelease(dict2), nullptr;

    const void* values[] = {dict1, dict2, dict3};
    CFArrayRef array = CFArrayCreate(kCFAllocatorDefault, values, 3, &kCFTypeArrayCallBacks);
    CFRelease(dict1), CFRelease(dict2), CFRelease(dict3);
    return array;
  }

  auto createMatcherCriteria(uint32_t page, uint32_t usage) -> CFDictionaryRef {
    CFNumberRef pageNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &page);
    if(!pageNumber) return nullptr;

    CFNumberRef usageNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage);
    if(!usageNumber) return CFRelease(pageNumber), nullptr;

    const void* keys[] = {CFSTR(kIOHIDDeviceUsagePageKey), CFSTR(kIOHIDDeviceUsageKey)};
    const void* values[] = {pageNumber, usageNumber};

    CFDictionaryRef dict = CFDictionaryCreate(
      kCFAllocatorDefault, keys, values, 2,
      &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks
    );
    CFRelease(pageNumber), CFRelease(usageNumber);
    return dict;
  }
};

auto deviceMatchingCallback(void* context, IOReturn result, void* sender, IOHIDDeviceRef device) -> void {
  ((InputJoypadIOKit*)context)->appendJoypad(device);
}

auto deviceRemovalCallback(void* context, IOReturn result, void* sender, IOHIDDeviceRef device) -> void {
  ((InputJoypadIOKit*)context)->removeJoypad(device);
}
