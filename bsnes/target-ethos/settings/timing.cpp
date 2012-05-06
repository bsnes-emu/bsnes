TimingSettings *timingSettings = nullptr;

TimingAdjustment::TimingAdjustment() {
  assign.setEnabled(false);
  assign.setText("Assign");
  analyze.setText("Analyze");
  stop.setEnabled(false);
  stop.setText("Stop");

  append(name, {40, 0});
  append(value, {100, 0}, 5);
  append(assign, {80, 0}, 5);
  append(spacer, {~0, 0});
  append(analyze, {80, 0}, 5);
  append(stop, {80, 0});
}

TimingSettings::TimingSettings() {
  title.setFont(application->titleFont);
  title.setText("Audiovisual Synchronization");
  videoAdjust.name.setText("Video:");
  videoAdjust.value.setText({config->timing.video});
  audioAdjust.name.setText("Audio:");
  audioAdjust.value.setText({config->timing.audio});

  append(title, {~0, 0}, 5);
  append(videoAdjust, {~0, 0}, 5);
  append(audioAdjust, {~0, 0}, 5);

  videoAdjust.value.onChange = [&] { videoAdjust.assign.setEnabled(true); };
  audioAdjust.value.onChange = [&] { audioAdjust.assign.setEnabled(true); };
  videoAdjust.assign.onActivate = [&] {
    config->timing.video = atof(videoAdjust.value.text());
    videoAdjust.value.setText({config->timing.video});
    videoAdjust.assign.setEnabled(false);
    utility->synchronizeDSP();
  };
  audioAdjust.assign.onActivate = [&] {
    config->timing.audio = atof(audioAdjust.value.text());
    audioAdjust.value.setText({config->timing.audio});
    audioAdjust.assign.setEnabled(false);
    utility->synchronizeDSP();
  };
  videoAdjust.analyze.onActivate = {&TimingSettings::analyzeVideoFrequency, this};
  audioAdjust.analyze.onActivate = {&TimingSettings::analyzeAudioFrequency, this};
  videoAdjust.stop.onActivate = audioAdjust.stop.onActivate = [&] { analysis.stop = true; };
}

void TimingSettings::analyzeVideoFrequency() {
  video.set(Video::Synchronize, true);
  audio.set(Audio::Synchronize, false);
  videoAdjust.stop.setEnabled(true);
  analyzeStart();
  do {
    uint32_t *output;
    unsigned pitch;
    if(video.lock(output, pitch, 16, 16)) {
      pitch >>= 2;
      for(unsigned y = 0; y < 16; y++) memset(output + y * pitch, 0, 4 * 16);
      video.unlock();
      video.refresh();
    }
  } while(analyzeTick("Video"));
  analyzeStop();
}

void TimingSettings::analyzeAudioFrequency() {
  video.set(Video::Synchronize, false);
  audio.set(Audio::Synchronize, true);
  audioAdjust.stop.setEnabled(true);
  analyzeStart();
  do {
    audio.sample(0, 0);
  } while(analyzeTick("Audio"));
  analyzeStop();
}

void TimingSettings::analyzeStart() {
  audio.clear();
  settings->setModal(true);
  settings->panelList.setEnabled(false);
  videoAdjust.analyze.setEnabled(false);
  audioAdjust.analyze.setEnabled(false);
  settings->setStatusText("Initiailizing ...");
  OS::processEvents();

  analysis.stop = false;
  analysis.seconds = 0;
  analysis.counter = 0;
  analysis.sample.reset();
  analysis.systemTime = time(0);
}

bool TimingSettings::analyzeTick(const string &type) {
  analysis.counter++;

  time_t systemTime = time(0);
  if(systemTime > analysis.systemTime) {
    analysis.systemTime = systemTime;
    OS::processEvents();

    if(analysis.seconds < 3) {
      analysis.seconds++;
    } else {
      analysis.sample.append(analysis.counter);
      uintmax_t sum = 0;
      for(auto &point : analysis.sample) sum += point;
      settings->setStatusText({
        type, " sample rate: ", (double)sum / analysis.sample.size(), "hz",
        " (", analysis.sample.size(), " sample points)"
      });
    }

    analysis.counter = 0;
  }

  return !analysis.stop;
}

void TimingSettings::analyzeStop() {
  video.set(Video::Synchronize, config->video.synchronize);
  audio.set(Audio::Synchronize, config->audio.synchronize);

  settings->panelList.setEnabled(true);
  videoAdjust.analyze.setEnabled(true);
  audioAdjust.analyze.setEnabled(true);
  videoAdjust.stop.setEnabled(false);
  audioAdjust.stop.setEnabled(false);
  settings->setModal(false);
}
