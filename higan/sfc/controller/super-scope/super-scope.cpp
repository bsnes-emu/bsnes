//The Super Scope is a light-gun: it detects the CRT beam cannon position,
//and latches the counters by toggling iobit. This only works on controller
//port 2, as iobit there is connected to the PPU H/V counter latch.
//(PIO $4201.d7)

//It is obviously not possible to perfectly simulate an IR light detecting
//a CRT beam cannon, hence this class will read the PPU raster counters.

//A Super Scope can still technically be used in port 1, however it would
//require manual polling of PIO ($4201.d6) to determine when iobit was written.
//Note that no commercial game ever utilizes a Super Scope in port 1.

SuperScope::SuperScope(bool port) : Controller(port) {
  create(Controller::Enter, 21'477'272);
  sprite = Emulator::video.createSprite(32, 32);
  sprite->setPixels(Resource::Sprite::CrosshairGreen);

  latched = 0;
  counter = 0;

  //center cursor onscreen
  x = 256 / 2;
  y = 240 / 2;

  trigger   = false;
  cursor    = false;
  turbo     = false;
  pause     = false;
  offscreen = false;

  oldturbo    = false;
  triggerlock = false;
  pauselock   = false;

  prev = 0;
}

SuperScope::~SuperScope() {
  Emulator::video.removeSprite(sprite);
}

auto SuperScope::main() -> void {
  uint next = cpu.vcounter() * 1364 + cpu.hcounter();

  if(!offscreen) {
    uint target = y * 1364 + (x + 24) * 4;
    if(next >= target && prev < target) {
      //CRT raster detected, toggle iobit to latch counters
      iobit(0);
      iobit(1);
    }
  }

  if(next < prev) {
    //Vcounter wrapped back to zero; update cursor coordinates for start of new frame
    int nx = platform->inputPoll(port, ID::Device::SuperScope, X);
    int ny = platform->inputPoll(port, ID::Device::SuperScope, Y);
    nx += x;
    ny += y;
    x = max(-16, min(256 + 16, nx));
    y = max(-16, min(240 + 16, ny));
    offscreen = (x < 0 || y < 0 || x >= 256 || y >= ppu.vdisp());
    sprite->setPosition(x * 2 - 16, y * 2 - 16);
    sprite->setVisible(true);
  }

  prev = next;
  step(2);
  synchronize(cpu);
}

auto SuperScope::data() -> uint2 {
  if(counter >= 8) return 1;

  if(counter == 0) {
    //turbo is a switch; toggle is edge sensitive
    bool newturbo = platform->inputPoll(port, ID::Device::SuperScope, Turbo);
    if(newturbo && !oldturbo) {
      turbo = !turbo;  //toggle state
      sprite->setPixels(turbo ? Resource::Sprite::CrosshairRed : Resource::Sprite::CrosshairGreen);
    }
    oldturbo = newturbo;

    //trigger is a button
    //if turbo is active, trigger is level sensitive; otherwise, it is edge sensitive
    trigger = false;
    bool newtrigger = platform->inputPoll(port, ID::Device::SuperScope, Trigger);
    if(newtrigger && (turbo || !triggerlock)) {
      trigger = true;
      triggerlock = true;
    } else if(!newtrigger) {
      triggerlock = false;
    }

    //cursor is a button; it is always level sensitive
    cursor = platform->inputPoll(port, ID::Device::SuperScope, Cursor);

    //pause is a button; it is always edge sensitive
    pause = false;
    bool newpause = platform->inputPoll(port, ID::Device::SuperScope, Pause);
    if(newpause && !pauselock) {
      pause = true;
      pauselock = true;
    } else if(!newpause) {
      pauselock = false;
    }

    offscreen = (x < 0 || y < 0 || x >= 256 || y >= ppu.vdisp());
  }

  switch(counter++) {
  case 0: return offscreen ? 0 : trigger;
  case 1: return cursor;
  case 2: return turbo;
  case 3: return pause;
  case 4: return 0;
  case 5: return 0;
  case 6: return offscreen;
  case 7: return 0;  //noise (1 = yes)
  }
}

auto SuperScope::latch(bool data) -> void {
  if(latched == data) return;
  latched = data;
  counter = 0;
}
