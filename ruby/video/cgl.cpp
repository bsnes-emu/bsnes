#define GL_ALPHA_TEST 0x0bc0
#include "opengl/opengl.hpp"

struct VideoCGL;

@interface RubyVideoCGL : NSOpenGLView {
@public
  VideoCGL* video;
}
-(id) initWith:(VideoCGL*)video pixelFormat:(NSOpenGLPixelFormat*)pixelFormat;
-(void) reshape;
@end

struct VideoCGL : VideoDriver, OpenGL {
  VideoCGL& self = *this;
  VideoCGL(Video& super) : VideoDriver(super) {}
  ~VideoCGL() { terminate(); }

  auto create() -> bool override {
    return initialize();
  }

  auto driver() -> string override { return "OpenGL 3.2"; }
  auto ready() -> bool override { return _ready; }

  auto hasContext() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }
  auto hasFlush() -> bool override { return true; }
  auto hasShader() -> bool override { return true; }

  auto setContext(uintptr context) -> bool override {
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool override {
    if(!view) return true;
    @autoreleasepool {
      [[view openGLContext] makeCurrentContext];
      int blocking = self.blocking;
      [[view openGLContext] setValues:&blocking forParameter:NSOpenGLCPSwapInterval];
    }
    return true;
  }

  auto setFlush(bool flush) -> bool override {
    return true;
  }

  auto setShader(string shader) -> bool override {
    OpenGL::setShader(shader);
    return true;
  }

  auto clear() -> void override {
    @autoreleasepool {
      [view lockFocus];
      OpenGL::clear();
      [[view openGLContext] flushBuffer];
      [view unlockFocus];
    }
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  auto release() -> void override {
  }

  auto output() -> void override {
    @autoreleasepool {
      if([view lockFocusIfCanDraw]) {
        auto area = [view convertRectToBacking:[view bounds]];
        OpenGL::outputWidth = area.size.width;
        OpenGL::outputHeight = area.size.height;
        OpenGL::output();
        [[view openGLContext] flushBuffer];
        if(self.flush) glFinish();
        [view unlockFocus];
      }
    }
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!self.context) return false;

    @autoreleasepool {
      NSOpenGLPixelFormatAttribute attributeList[] = {
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADoubleBuffer,
        0
      };

      auto context = (NSView*)self.context;
      auto size = [context frame].size;
      auto format = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributeList] autorelease];
      auto openGLContext = [[[NSOpenGLContext alloc] initWithFormat:format shareContext:nil] autorelease];

      view = [[RubyVideoCGL alloc] initWith:this pixelFormat:format];
      [view setOpenGLContext:openGLContext];
      [view setFrame:NSMakeRect(0, 0, size.width, size.height)];
      [view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
      [view setWantsBestResolutionOpenGLSurface:YES];
      [context addSubview:view];
      [openGLContext setView:view];

      [view lockFocus];

      OpenGL::initialize();

      int blocking = self.blocking;
      [[view openGLContext] setValues:&blocking forParameter:NSOpenGLCPSwapInterval];

      [view unlockFocus];
    }

    clear();
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;
    OpenGL::terminate();

    if(!view) return;
    @autoreleasepool {
      [view removeFromSuperview];
      [view release];
      view = nil;
    }
  }

  RubyVideoCGL* view = nullptr;

  bool _ready = false;
};

@implementation RubyVideoCGL : NSOpenGLView

-(id) initWith:(VideoCGL*)videoPointer pixelFormat:(NSOpenGLPixelFormat*)pixelFormat {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0) pixelFormat:pixelFormat]) {
    video = videoPointer;
  }
  return self;
}

-(void) reshape {
  video->output();
}

@end
