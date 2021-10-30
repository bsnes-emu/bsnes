#pragma once

struct InputMouseQuartz {
  Input& input;
  InputMouseQuartz(Input& input) : input(input) {}

  shared_pointer<HID::Mouse> hid{new HID::Mouse};

  bool mouseAcquired = false;

  auto acquire() -> bool {
    if(!mouseAcquired) {
      CGError error = CGAssociateMouseAndMouseCursorPosition(NO);
      if(error != kCGErrorSuccess) return false;
      [NSCursor hide];

      mouseAcquired = true;
    }
    return true;
  }

  auto acquired() -> bool {
    return mouseAcquired;
  }

  auto release() -> bool {
    if(mouseAcquired) {
      CGError error = CGAssociateMouseAndMouseCursorPosition(YES);
      if(error != kCGErrorSuccess) return false;
      [NSCursor unhide];

      mouseAcquired = false;
    }
    return true;
  }

  auto assign(uint groupID, uint inputID, int16_t value) -> void {
    auto& group = hid->group(groupID);
    if(group.input(inputID).value() == value) return;
    input.doChange(hid, groupID, inputID, group.input(inputID).value(), value);
    group.input(inputID).setValue(value);
  }

  auto poll(vector<shared_pointer<HID::Device>>& devices) -> void {
    int deltaX, deltaY;
    CGGetLastMouseDelta(&deltaX, &deltaY);

    assign(HID::Mouse::GroupID::Axis, 0, deltaX);
    assign(HID::Mouse::GroupID::Axis, 1, deltaY);

    NSUInteger buttons = [NSEvent pressedMouseButtons];

    assign(HID::Mouse::GroupID::Button, 0, !!(buttons & (1 << 0)));
    assign(HID::Mouse::GroupID::Button, 2, !!(buttons & (1 << 1)));
    assign(HID::Mouse::GroupID::Button, 1, !!(buttons & (1 << 2)));
    assign(HID::Mouse::GroupID::Button, 4, !!(buttons & (1 << 3)));
    assign(HID::Mouse::GroupID::Button, 3, !!(buttons & (1 << 4)));

    devices.append(hid);
  }

  auto initialize() -> bool {
    hid->setVendorID(HID::Mouse::GenericVendorID);
    hid->setProductID(HID::Mouse::GenericProductID);
    hid->setPathID(0);

    hid->axes().append("X");
    hid->axes().append("Y");

    hid->buttons().append("Left");
    hid->buttons().append("Middle");
    hid->buttons().append("Right");
    hid->buttons().append("Up");
    hid->buttons().append("Down");

    return true;
  }

  auto terminate() -> void {
    release();
  }
};
