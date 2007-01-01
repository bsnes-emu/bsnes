void load_video_settings(uint profile) {
stringarray line, part;
  if(profile >= VIDEO_PROFILE_COUNT)profile = 0;

  switch(profile) {
  case 0: strcpy(line, config::video.profile_win.strget());  break;
  case 1: strcpy(line, config::video.profile_full.strget()); break;
  }

  split(part, ";", line);
VideoSettings *v = &video_settings[profile];
int i = 0;
  v->software_filter      = strdec(part[i++]);
  v->hardware_filter      = strdec(part[i++]);
  v->video_standard       = strdec(part[i++]);
  v->multiplier           = strdec(part[i++]);
  v->correct_aspect_ratio = !strcmp(part[i++], "true");
  v->enable_scanlines     = !strcmp(part[i++], "true");
  v->manual_render_size   = !strcmp(part[i++], "true");
  v->render_width         = strdec(part[i++]);
  v->render_height        = strdec(part[i++]);
  v->fullscreen           = (profile == 1);
  v->triple_buffering     = !strcmp(part[i++], "true");
  v->resolution_width     = strdec(part[i++]);
  v->resolution_height    = strdec(part[i++]);
  v->refresh_rate         = strdec(part[i++]);

//0,1 = 1x; 2 = 2x, 3 = 3x, ..., 8+ = 8x
  v->multiplier = minmax<1, 8>(v->multiplier);
  if(v->render_width  < 256) { v->render_width  = 256; }
  if(v->render_height < 224) { v->render_height = 224; }
}

void save_video_settings(uint profile) {
stringarray line;
char   part[64];
  if(profile >= VIDEO_PROFILE_COUNT)profile = 0;

VideoSettings *v = &video_settings[profile];
  strcpy(line, "");
  sprintf(part, "%d", v->software_filter);                         strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->hardware_filter);                         strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->video_standard);                          strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->multiplier);                              strcat(line, part); strcat(line, ";");
  sprintf(part, "%s", v->correct_aspect_ratio ? "true" : "false"); strcat(line, part); strcat(line, ";");
  sprintf(part, "%s", v->enable_scanlines     ? "true" : "false"); strcat(line, part); strcat(line, ";");
  sprintf(part, "%s", v->manual_render_size   ? "true" : "false"); strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->render_width);                            strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->render_height);                           strcat(line, part); strcat(line, ";");
  sprintf(part, "%s", v->triple_buffering     ? "true" : "false"); strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->resolution_width);                        strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->resolution_height);                       strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->refresh_rate);                            strcat(line, part);

  switch(profile) {
  case 0: config::video.profile_win.strset(strptr(line));  break;
  case 1: config::video.profile_full.strset(strptr(line)); break;
  }
}

void Video::update_video_settings() {
uint profile = uint(config::video.profile);
  if(profile >= VIDEO_PROFILE_COUNT)profile = 0;

  load_video_settings(profile);
VideoSettings *v = &video_settings[profile];
  snes.set_video_format(v->software_filter, v->video_standard, SNES::PIXELFORMAT_RGB565);

  if(v->manual_render_size == true) {
    settings.render_width  = v->render_width;
    settings.render_height = v->render_height;
  } else {
    settings.render_width  = 256;
    settings.render_height = (v->video_standard == SNES::VIDEOSTANDARD_NTSC) ? 224 : 239;

    settings.render_width  *= v->multiplier;
    settings.render_height *= v->multiplier;

    if(v->correct_aspect_ratio == true) {
    //TODO: support non-4:3 resolutions (eg 1280x1024 -- 5:4)
      settings.render_width = (uint)( (double)settings.render_height / 3.0 * 4.0 );
    }
  }

  settings.fullscreen       = v->fullscreen;
  settings.triple_buffering = v->triple_buffering;

  settings.hardware_filter  = v->hardware_filter;
  settings.enable_scanlines = v->enable_scanlines;

  if(v->fullscreen == true) {
    settings.resolution_width  = (v->resolution_width)  ? v->resolution_width  : screen_width();
    settings.resolution_height = (v->resolution_height) ? v->resolution_height : screen_height();
    settings.refresh_rate      = v->refresh_rate;
  } else {
    settings.resolution_width  = settings.render_width;
    settings.resolution_height = settings.render_height;
    settings.refresh_rate      = 0;
  }

  if(settings.render_width  <= settings.resolution_width) {
    settings.rx = (settings.resolution_width  - settings.render_width)  >> 1;
    settings.rw = settings.render_width;
  } else {
    settings.rx = 0;
    settings.rw = settings.resolution_width;
  }

  if(settings.render_height <= settings.resolution_height) {
    settings.ry = (settings.resolution_height - settings.render_height) >> 1;
    settings.rh = settings.render_height;
  } else {
    settings.ry = 0;
    settings.rh = settings.resolution_height;
  }
}

Video::Video() {
  for(int i = 0; i < VIDEO_PROFILE_COUNT; i++) {
    load_video_settings(i);
  }
}

Video::~Video() {
  for(int i = 0; i < VIDEO_PROFILE_COUNT; i++) {
    save_video_settings(i);
  }
}
