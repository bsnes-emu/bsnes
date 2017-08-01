FightingPad::FightingPad(uint port) : Controller(port) {
  create(Controller::Enter, 1'000'000);
}

auto FightingPad::main() -> void {
  if(timeout) {
    timeout--;
  } else {
    counter = 0;
  }
  step(1);
  synchronize(cpu);
}

auto FightingPad::readData() -> uint8 {
  uint6 data;

  if(select == 0) {
    if(counter == 0 || counter == 1 || counter == 4) {
      data.bit(0) = platform->inputPoll(port, ID::Device::FightingPad, Up);
      data.bit(1) = platform->inputPoll(port, ID::Device::FightingPad, Down);
      data.bits(2,3) = ~0;
    }

    if(counter == 2) {
      data.bits(0,3) = ~0;  //controller type detection
    }

    if(counter == 3) {
      data.bits(0,3) = 0;
    }

    data.bit(4) = platform->inputPoll(port, ID::Device::FightingPad, A);
    data.bit(5) = platform->inputPoll(port, ID::Device::FightingPad, Start);
  } else {
    if(counter == 0 || counter == 1 || counter == 2 || counter == 4) {
      data.bit(0) = platform->inputPoll(port, ID::Device::FightingPad, Up);
      data.bit(1) = platform->inputPoll(port, ID::Device::FightingPad, Down);
      data.bit(2) = platform->inputPoll(port, ID::Device::FightingPad, Left);
      data.bit(3) = platform->inputPoll(port, ID::Device::FightingPad, Right);
      data.bit(4) = platform->inputPoll(port, ID::Device::FightingPad, B);
      data.bit(5) = platform->inputPoll(port, ID::Device::FightingPad, C);
    }

    if(counter == 3) {
      data.bit(0) = platform->inputPoll(port, ID::Device::FightingPad, Z);
      data.bit(1) = platform->inputPoll(port, ID::Device::FightingPad, Y);
      data.bit(2) = platform->inputPoll(port, ID::Device::FightingPad, X);
      data.bit(3) = platform->inputPoll(port, ID::Device::FightingPad, Mode);
      data.bits(4,5) = 0;
    }
  }

  data = ~data;
  return latch << 7 | select << 6 | data;
}

auto FightingPad::writeData(uint8 data) -> void {
  if(!select && data.bit(6)) {  //0->1 transition
    if(++counter == 5) counter = 0;
  }

  select = data.bit(6);
  latch = data.bit(7);
  timeout = 1600;  //~1.6ms
}
