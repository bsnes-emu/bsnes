void Status::update() {
  string output;

  if(queue[0].valid == true) {
    output = queue[0].text;
    time_t currenttime = time(0);
    if((int)(currenttime - displaytime) >= 3) {
      displaytime = currenttime;

      //message displayed for 3 seconds or more, remove from queue
      for(unsigned i = 0; i < 15; i++) {
        queue[i].valid = queue[i + 1].valid;
        queue[i].text  = queue[i + 1].text;
      }
      queue[15].valid = false;
      queue[15].text  = "";
    }
  } else if(!cartridge.loaded()) {
    output = "";
  } else if(app.pause || app.autopause) {
    output = "Paused";
  } else if(ppu.status.frames_updated) {
    ppu.status.frames_updated = false;

    unsigned max_framerate = snes.region() == SNES::NTSC ? 60 : 50;
    switch(config::system.emulation_speed) {
      case 0: max_framerate = unsigned(0.50 * max_framerate); break;
      case 1: max_framerate = unsigned(0.75 * max_framerate); break;
      case 2: max_framerate = unsigned(1.00 * max_framerate); break;
      case 3: max_framerate = unsigned(1.50 * max_framerate); break;
      case 4: max_framerate = unsigned(2.00 * max_framerate); break;
      case 5: max_framerate = 0; break;
    }

    output = string() << cartridge.info.filename << " : " << (int)ppu.status.frames_executed;
    if(max_framerate != 0) {
      output << " / ";
      output << (int)max_framerate;
    }
  } else {
    //no operation
    return;
  }

  if(text != output) {
    //refresh status text if it has changed since last redraw
    text = output;
    window.status.set_text(text);
  }
}

void Status::flush() {
  for(unsigned i = 0; i < 16; i++) {
    queue[i].valid = false;
    queue[i].text  = "";
  }
}

bool Status::enqueue(const char *message) {
  unsigned index = 0;
  while(index < 16) {
    if(queue[index].valid == false) break;
    index++;
  }
  //queue full?
  if(index >= 16) return false;

  queue[index].valid = true;
  queue[index].text  = message;
  displaytime = time(0);
  update();

  return true;
}

Status::Status() : window(window_main) {
  flush();
}
