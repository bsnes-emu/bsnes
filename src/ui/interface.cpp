SNESInterface snesinterface;

//video

bool SNESInterface::video_lock(uint16 *&data, uint &pitch) {
  return uiVideo->lock(data, pitch);
}

void SNESInterface::video_unlock() {
  uiVideo->unlock();
}

void SNESInterface::video_refresh() {
  if(r_ppu->renderer_enabled() == true) { uiVideo->update(); }
}

//audio

void SNESInterface::audio_sample(uint16 l_sample, uint16 r_sample) {
  uiAudio->run( (l_sample) | (r_sample << 16) );
}

//input

void SNESInterface::input_poll() {
  uiInput->poll();
}

bool SNESInterface::input_poll(uint deviceid, uint button) {
  return uiInput->get_status(deviceid, button);
}
