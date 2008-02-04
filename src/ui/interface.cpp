/*****
 * SNES Interface class
 *
 * Interfaces SNES core with platform-specific functionality
 * (video, audio, input, ...)
 *****/

SNESInterface snesinterface;

//video

bool SNESInterface::video_lock(uint16 *&data, uint &pitch) {
  return video.lock(data, pitch);
}

void SNESInterface::video_unlock() {
  video.unlock();
}

static uint frameskip_counter = 0;

void SNESInterface::video_refresh() {
  if(ppu.renderer_enabled() == true) {
  SNES::video_info vi;
    snes.get_video_info(&vi);
    video.refresh(vi.width, vi.height);
  }

  frameskip_counter++;
  frameskip_counter %= (uint(config::video.frameskip) + 1);
  ppu.enable_renderer(frameskip_counter == 0);
}

//audio

void SNESInterface::audio_sample(uint16 l_sample, uint16 r_sample) {
  if(config::audio.mute == true) {
    l_sample = 0;
    r_sample = 0;
  }
  audio.sample(l_sample, r_sample);
}

//input

void SNESInterface::input_poll() {
  if(input_ready && input_ready() == false) {
    input.clear();
  } else {
    input.poll();
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
