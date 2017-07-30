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

  auto ready() -> bool { return _ready; }

  auto context() -> uintptr { return (uintptr)_context; }
  auto blocking() -> bool { return _blocking; }
  auto smooth() -> bool { return _smooth; }
  auto shader() -> string { return _shader; }

  auto setContext(uintptr context) -> bool {
    if(_context == (NSView*)context) return true;
    _context = (NSView*)context;
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool {
    if(_blocking == blocking) return true;
    _blocking = blocking;
    if(!view) return true;
    @autoreleasepool {
      [[view openGLContext] makeCurrentContext];
      int blocking = _blocking;
      [[view openGLContext] setValues:&blocking forParameter:NSOpenGLCPSwapInterval];
    }
    return true;
  }

  auto setSmooth(bool smooth) -> bool {
    if(_smooth == smooth) return true;
    _smooth = smooth;
    if(!_shader) OpenGL::filter = _smooth ? GL_LINEAR : GL_NEAREST;
    return true;
  }

  auto setShader(string shader) -> bool {
    if(_shader == shader) return true;
    OpenGL::shader(_shader = shader);
    if(!_shader) OpenGL::filter = _smooth ? GL_LINEAR : GL_NEAREST;
    return true;
  }

  auto clear() -> void {
    if(!ready()) return;
    @autoreleasepool {
      [view lockFocus];
      OpenGL::clear();
      [[view openGLContext] flushBuffer];
      [view unlockFocus];
    }
  }

  auto lock(uint32_t*& data, uint& pitch, uint width, uint height) -> bool {
    if(!ready()) return false;
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  auto unlock() -> void {
    if(!ready()) return;
  }

  auto output() -> void {
    if(!ready()) return;
    @autoreleasepool {
      if([view lockFocusIfCanDraw]) {
        auto area = [view frame];
        OpenGL::outputWidth = area.size.width;
        OpenGL::outputHeight = area.size.height;
        OpenGL::output();
        [[view openGLContext] flushBuffer];
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

      auto size = [_context frame].size;
      auto format = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributeList] autorelease];
      auto context = [[[NSOpenGLContext alloc] initWithFormat:format shareContext:nil] autorelease];

      view = [[RubyVideoCGL alloc] initWith:this pixelFormat:format];
      [view setOpenGLContext:context];
      [view setFrame:NSMakeRect(0, 0, size.width, size.height)];
      [view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
      [_context addSubview:view];
      [context setView:view];

      [view lockFocus];

      OpenGL::initialize();

      int blocking = _blocking;
      [[view openGLContext] setValues:&blocking forParameter:NSOpenGLCPSwapInterval];

      [view unlockFocus];
    }

    clear();
    return _ready = true;
  }

  auto term() -> void {
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
  NSView* _context = nullptr;
  bool _blocking = false;
  bool _smooth = true;
  string _shader;
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
