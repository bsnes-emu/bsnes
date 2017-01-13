Justifier::Justifier(bool port, bool chained):
Controller(port),
chained(chained),
device(!chained ? ID::Device::Justifier : ID::Device::Justifiers)
{
  create(Controller::Enter, 21'477'272);
  latched = 0;
  counter = 0;
  active = 0;
  prev = 0;

  player1.sprite = Emulator::video.createSprite(32, 32);
  player1.sprite->setPixels(Resource::Sprite::CrosshairGreen);
  player1.x = 256 / 2;
  player1.y = 240 / 2;
  player1.trigger = false;
  player2.start = false;

  player2.sprite = Emulator::video.createSprite(32, 32);
  player2.sprite->setPixels(Resource::Sprite::CrosshairRed);
  player2.x = 256 / 2;
  player2.y = 240 / 2;
  player2.trigger = false;
  player2.start = false;

  if(chained == false) {
    player2.x = -1;
    player2.y = -1;
  } else {
    player1.x -= 16;
    player2.x += 16;
  }
}

Justifier::~Justifier() {
  Emulator::video.removeSprite(player1.sprite);
  Emulator::video.removeSprite(player2.sprite);
}

auto Justifier::main() -> void {
  uint next = cpu.vcounter() * 1364 + cpu.hcounter();

  int x = (active == 0 ? player1.x : player2.x), y = (active == 0 ? player1.y : player2.y);
  bool offscreen = (x < 0 || y < 0 || x >= 256 || y >= ppu.vdisp());

  if(!offscreen) {
    uint target = y * 1364 + (x + 24) * 4;
    if(next >= target && prev < target) {
      //CRT raster detected, toggle iobit to latch counters
      iobit(0);
      iobit(1);
    }
  }

  if(next < prev) {
    int nx1 = platform->inputPoll(port, device, 0 + X);
    int ny1 = platform->inputPoll(port, device, 0 + Y);
    nx1 += player1.x;
    ny1 += player1.y;
    player1.x = max(-16, min(256 + 16, nx1));
    player1.y = max(-16, min(240 + 16, ny1));
    player1.sprite->setPosition(player1.x * 2 - 16, player1.y * 2 - 16);
    player1.sprite->setVisible(true);
  }

  if(next < prev && chained) {
    int nx2 = platform->inputPoll(port, device, 4 + X);
    int ny2 = platform->inputPoll(port, device, 4 + Y);
    nx2 += player2.x;
    ny2 += player2.y;
    player2.x = max(-16, min(256 + 16, nx2));
    player2.y = max(-16, min(240 + 16, ny2));
    player2.sprite->setPosition(player2.x * 2 - 16, player2.y * 2 - 16);
    player2.sprite->setVisible(true);
  }

  prev = next;
  step(2);
  synchronize(cpu);
}

auto Justifier::data() -> uint2 {
  if(counter >= 32) return 1;

  if(counter == 0) {
    player1.trigger = platform->inputPoll(port, device, 0 + Trigger);
    player1.start   = platform->inputPoll(port, device, 0 + Start);
  }

  if(counter == 0 && chained) {
    player2.trigger = platform->inputPoll(port, device, 4 + Trigger);
    player2.start   = platform->inputPoll(port, device, 4 + Start);
  }

  switch(counter++) {
  case  0: return 0;
  case  1: return 0;
  case  2: return 0;
  case  3: return 0;
  case  4: return 0;
  case  5: return 0;
  case  6: return 0;
  case  7: return 0;
  case  8: return 0;
  case  9: return 0;
  case 10: return 0;
  case 11: return 0;

  case 12: return 1;  //signature
  case 13: return 1;  // ||
  case 14: return 1;  // ||
  case 15: return 0;  // ||

  case 16: return 0;
  case 17: return 1;
  case 18: return 0;
  case 19: return 1;
  case 20: return 0;
  case 21: return 1;
  case 22: return 0;
  case 23: return 1;

  case 24: return player1.trigger;
  case 25: return player2.trigger;
  case 26: return player1.start;
  case 27: return player2.start;
  case 28: return active;

  case 29: return 0;
  case 30: return 0;
  case 31: return 0;
  }
}

auto Justifier::latch(bool data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;
  if(latched == 0) active = !active;  //toggle between both controllers, even when unchained
}
