#include "filter.cpp"

void SNES::set_video_format(uint filter, uint video_standard, uint pixel_format) {
//only make changes at the start of a new frame
  video_format.filter         = filter;
  video_format.video_standard = video_standard;
  video_format.pixel_format   = pixel_format;
  video_format.modified       = true;
}

/*****
 * Internal function called at the start of each frame.
 * Use SNES::set_video_format to modify these options.
 *****/
void SNES::update_video_format() {
  if(video_format.modified == false)return;
  video_format.modified = false;

  video.filter = video_format.filter;
  SafeDelete(video_filter);
  switch(video.filter) {
  default:
  case VIDEOFILTER_DIRECT:  video_filter = new DirectVideoFilter();  break;
  case VIDEOFILTER_NTSC:    video_filter = new NtscVideoFilter();    break;
  case VIDEOFILTER_HQ2X:    video_filter = new HQ2xVideoFilter();    break;
  case VIDEOFILTER_SCALE2X: video_filter = new Scale2xVideoFilter(); break;
  }

  video.video_standard = video_format.video_standard;
  video.pixel_format   = video_format.pixel_format;
  update_color_lookup_table();
}

void SNES::get_video_info(video_info *info) {
  info->filter         = video.filter;
  info->video_standard = video.video_standard;
  info->pixel_format   = video.pixel_format;
  info->width          = video.width;
  info->height         = video.height;
}

void SNES::video_update() {
  if(r_ppu->renderer_enabled()) {
    update_video_format();

    video.ppu_data = (uint16*)r_ppu->output;
//  video_normalize();

    switch(video.video_standard) {
    default:
    case VIDEOSTANDARD_NTSC:
      video.raster_width  = 256;
      video.raster_height = 224;
      video.ppu_data += (int(r_cpu->overscan()) << 13) + 1024;
      break;
    case VIDEOSTANDARD_PAL:
      video.raster_width  = 256;
      video.raster_height = 239;
      video.ppu_data += 1024;
      break;
    }

    if(video.frame_hires)     { video.raster_width  <<= 1; }
    if(video.frame_interlace) { video.raster_height <<= 1; }

    if(snesinterface.video_lock(video.data, video.pitch) == true) {
      video_filter->run(color_lookup_table, video.ppu_data,
                        video.raster_width, video.raster_height,
                        video.raster_height <= 240 ? 2048 : 1024,
                        video.data, 512, 480, video.pitch,
                        512, 480, video.width, video.height,
                        video.raster_height <= 240 ? (pline_width + 1) : (iline_width + 2));
      snesinterface.video_unlock();
    }
  }

  snesinterface.video_refresh();

  video.frame_hires     = false;
  video.frame_interlace = false;
}

void SNES::video_scanline() {
int y = r_cpu->vcounter();
int o = (video.video_standard == VIDEOSTANDARD_NTSC) ? (int(r_cpu->overscan()) << 3) : 0;
  if(y <= (0 + o) || y >= (225 + o))return;
  y -= o;

PPU::scanline_info si;
  r_ppu->get_scanline_info(&si);

  pline_width[y] = iline_width[y * 2 + int(r_cpu->interlace_field())] =
    (si.hires == false) ? 256 : 512;
  video.frame_hires      |= si.hires;
  video.frame_interlace  |= si.interlace;
}

void SNES::video_init() {
  for(int i = 0; i < 240; i++)pline_width[i] = 256;
  for(int i = 0; i < 480; i++)iline_width[i] = 256;
  video.frame_hires     = false;
  video.frame_interlace = false;

  video.raster_data = (uint16*)malloc(512 * 480 * sizeof(uint16));
  memset(video.raster_data, 0, 512 * 480 * sizeof(uint16));
  video_filter = 0;
  set_video_format(VIDEOFILTER_DIRECT, VIDEOSTANDARD_NTSC, PIXELFORMAT_RGB565);
  update_video_format();
}
