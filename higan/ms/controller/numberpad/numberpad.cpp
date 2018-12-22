NumberPad::NumberPad(uint port) : Controller(port) {
}

auto NumberPad::readData() -> uint8 {
  const uint device = ID::Device::NumberPad;
  uint8 data = 0xff;
  if(select == 0) {
         if(platform->inputPoll(port, device, One  )) data.bits(0,3) = 0b1101;
    else if(platform->inputPoll(port, device, Two  )) data.bits(0,3) = 0b0111;
    else if(platform->inputPoll(port, device, Three)) data.bits(0,3) = 0b1100;
    else if(platform->inputPoll(port, device, Four )) data.bits(0,3) = 0b0010;
    else if(platform->inputPoll(port, device, Five )) data.bits(0,3) = 0b0011;
    else if(platform->inputPoll(port, device, Six  )) data.bits(0,3) = 0b1110;
    else if(platform->inputPoll(port, device, Seven)) data.bits(0,3) = 0b0101;
    else if(platform->inputPoll(port, device, Eight)) data.bits(0,3) = 0b0001;
    else if(platform->inputPoll(port, device, Nine )) data.bits(0,3) = 0b1011;
    else if(platform->inputPoll(port, device, Star )) data.bits(0,3) = 0b1001;
    else if(platform->inputPoll(port, device, Zero )) data.bits(0,3) = 0b1010;
    else if(platform->inputPoll(port, device, Pound)) data.bits(0,3) = 0b0110;
    data.bit(6) = !platform->inputPoll(port, device, R);
  } else {
    data.bit(0) = !platform->inputPoll(port, device, Up);
    data.bit(1) = !platform->inputPoll(port, device, Right);
    data.bit(2) = !platform->inputPoll(port, device, Down);
    data.bit(3) = !platform->inputPoll(port, device, Left);
    data.bit(6) = !platform->inputPoll(port, device, L);
  }
  return data;
}

auto NumberPad::writeData(uint8 data) -> void {
  select = data.bit(0);
}
