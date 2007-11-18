/*****
 * SNES Interface class
 *
 * Interfaces SNES core with platform-specific functionality
 * (video, audio, input, ...)
 *****/

SNESInterface snesinterface;

//video

bool SNESInterface::video_lock(uint16 *&data, uint &pitch) {
  return uiVideo->lock(data, pitch);
}

void SNESInterface::video_unlock() {
  uiVideo->unlock();
}

static uint frameskip_counter = 0;

void SNESInterface::video_refresh() {
  if(ppu.renderer_enabled() == true) {
  SNES::video_info vi;
    snes.get_video_info(&vi);
    uiVideo->refresh(vi.width, vi.height);
  }

  frameskip_counter++;
  frameskip_counter %= (uint(config::video.frameskip) + 1);
  ppu.enable_renderer(frameskip_counter == 0);
}

//audio

void SNESInterface::audio_sample(uint16 l_sample, uint16 r_sample) {
  uiAudio->sample(l_sample, r_sample);
}

//input

void SNESInterface::input_poll() {
  if(input_ready && input_ready() == false) {
    uiInput->clear_input();
  } else {
    uiInput->poll();
  }
  input_manager.poll();
}

bool SNESInterface::input_poll(uint deviceid, uint button) {
  return input_manager.get_status(deviceid, button);
}

//core

void SNESInterface::init() {
  input_manager.bind();
}

void SNESInterface::term() {
}
