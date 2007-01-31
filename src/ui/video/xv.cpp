bool VideoXv::lock(uint16 *&data, uint &pitch) {
  pitch = 1024 * 2;
  data  = buffer;
  return data;
}

void VideoXv::unlock() {
}

void VideoXv::redraw() {
Window dw;
int d0;
unsigned int d1;
unsigned int width, height;
  XGetGeometry(display, window, &dw, &d0, &d0, &width, &height, &d1, &d1);

SNES::video_info vi;
  snes.get_video_info(&vi);

uint16 *input  = (uint16*)buffer;
uint32 *output = (uint32*)xvimage->data;
  for(int y = 0; y < vi.height; y++) {
    for(int x = 0; x < vi.width; x++) {
    uint32 p = *input++;
      *output++ = ((p & 0xf800) << 8) | ((p & 0x07e0) << 5) | ((p & 0x001f) << 3);
    }
    input  += 1024 - vi.width;
    output += 1024 - vi.width;
  }

  XvShmPutImage(display, xv_port, window, gc, xvimage,
    0, 0, vi.width, vi.height,
    0, 0, width, height,
    true);
}

void VideoXv::update() {
  redraw();
}

void VideoXv::init() {
}

void VideoXv::term() {
}

VideoXv::VideoXv(unsigned long handle) {
  buffer  = (uint16*)malloc(1024 * 1024 * 2);
  window  = handle;
  display = XOpenDisplay(0);
  screen  = DefaultScreen(display);
  gc      = XCreateGC(display, window, 0, 0);

XVisualInfo visual_info;
         if(XMatchVisualInfo(display, screen, 24, TrueColor,   &visual_info)) {
  } else if(XMatchVisualInfo(display, screen, 16, TrueColor,   &visual_info)) {
  } else if(XMatchVisualInfo(display, screen, 15, TrueColor,   &visual_info)) {
  } else if(XMatchVisualInfo(display, screen,  8, PseudoColor, &visual_info)) {
  } else if(XMatchVisualInfo(display, screen,  8, GrayScale,   &visual_info)) {
  } else if(XMatchVisualInfo(display, screen,  8, StaticGray,  &visual_info)) {
  } else if(XMatchVisualInfo(display, screen,  1, StaticGray,  &visual_info)) {
  } else {
    printf("VideoXv: unable to find suitable video display.\n");
  }

  if(!XShmQueryExtension(display)) {
    printf("VideoXv: XShm extension not found.\n");
  }

  xv_port = -1;
XvAdaptorInfo *adaptor_info;
unsigned int adaptor_count;
  XvQueryAdaptors(display, DefaultRootWindow(display), &adaptor_count, &adaptor_info);
  for(uint i = 0; i < adaptor_count; i++) {
    if(adaptor_info[i].num_ports > 1) {
    //not certain if this is correct ... xv_port with only one port fails to work on
    //geforce 6600le, so find the first xv_port with more than one port.
      xv_port = adaptor_info[i].base_id;
      break;
    }
  }
  XvFreeAdaptorInfo(adaptor_info);
  if(xv_port == -1) {
    printf("VideoXv: failed to find valid XvPort.\n");
  }

//0x00000003 = 32-bit X8R8G8B8
  xvimage = XvShmCreateImage(display, xv_port, 0x00000003, 0, 1024, 1024, &shminfo);
  shminfo.shmid    = shmget(IPC_PRIVATE, xvimage->data_size, IPC_CREAT | 0777);
  shminfo.shmaddr  = xvimage->data = (char*)shmat(shminfo.shmid, 0, 0);
  shminfo.readOnly = false;
  if(!XShmAttach(display, &shminfo)) {
    printf("VideoXv: XShmAttach failed.\n");
  }
}

VideoXv::~VideoXv() {
  XShmDetach(display, &shminfo);
}
