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

struct VideoCGL : Video, OpenGL {
  VideoCGL() { initialize(); }
  ~VideoCGL() { terminate(); }

  auto driver() -> string override { return "OpenGL"; }
  auto ready() -> bool override { return _ready; }

  auto hasContext() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }
  auto hasFlush() -> bool override { return true; }
  auto hasSmooth() -> bool override { return true; }
  auto hasShader() -> bool override { return true; }

  auto setContext(uintptr context) -> bool override {
    if(context == Video::context()) return true;
    if(!Video::setContext(context)) return false;
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool override {
    if(blocking == Video::blocking()) return true;
    if(!Video::setBlocking(blocking)) return false;
    if(!view) return true;
    @autoreleasepool {
      [[view openGLContext] makeCurrentContext];
      int blocking = _blocking;
      [[view openGLContext] setValues:&blocking forParameter:NSOpenGLCPSwapInterval];
    }
    return true;
  }

  auto setFlush(bool flush) -> bool override {
    if(flush == Video::flush()) return true;
    if(!Video::setFlush(flush)) return false;
    return true;
  }

  auto setSmooth(bool smooth) -> bool override {
    if(smooth == Video::smooth()) return true;
    if(!Video::setSmooth(smooth)) return false;
    if(!shader()) OpenGL::filter = smooth ? GL_LINEAR : GL_NEAREST;
    return true;
  }

  auto setShader(string shader) -> bool override {
    if(shader == Video::shader()) return true;
    if(!Video::setShader(shader)) return false;
    OpenGL::setShader(shader);
    if(!shader) OpenGL::filter = smooth() ? GL_LINEAR : GL_NEAREST;
    return true;
  }

  auto clear() -> void override {
    if(!ready()) return;
    @autoreleasepool {
      [view lockFocus];
      OpenGL::clear();
      [[view openGLContext] flushBuffer];
      [view unlockFocus];
    }
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    if(!ready()) return false;
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  auto release() -> void override {
    if(!ready()) return;
  }

  auto output() -> void override {
    if(!ready()) return;
    @autoreleasepool {
      if([view lockFocusIfCanDraw]) {
        auto area = [view convertRectToBacking:[view bounds]];
        OpenGL::outputWidth = area.size.width;
        OpenGL::outputHeight = area.size.height;
        OpenGL::output();
        [[view openGLContext] flushBuffer];
        if(flush()) glFinish();
        [view unlockFocus];
      }
    }
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!_context) return false;

    @autoreleasepool {
      NSOpenGLPixelFormatAttribute attributeList[] = {
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADoubleBuffer,
        0
      };

      auto context = (NSView*)_context;
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

      int blocking = _blocking;
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
