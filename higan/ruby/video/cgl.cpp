#include "opengl.hpp"

namespace ruby {
  class pVideoCGL;
}

@interface RubyVideoCGL : NSOpenGLView {
@public
  ruby::pVideoCGL *video;
}
-(id) initWith:(ruby::pVideoCGL*)video pixelFormat:(NSOpenGLPixelFormat*)pixelFormat;
-(void) reshape;
@end

namespace ruby {

class pVideoCGL : public OpenGL {
public:
  RubyVideoCGL *view;

  struct {
    NSView *handle;

    bool synchronize;
    unsigned filter;
    string shader;

    unsigned width;
    unsigned height;
  } settings;

  bool cap(const string &name) {
    if(name == Video::Handle) return true;
    if(name == Video::Synchronize) return true;
    if(name == Video::Filter) return true;
    if(name == Video::Shader) return true;
    return false;
  }

  any get(const string &name) {
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    if(name == Video::Synchronize) return settings.synchronize;
    if(name == Video::Filter) return settings.filter;
    return false;
  }

  bool set(const string &name, const any &value) {
    if(name == Video::Handle) {
      settings.handle = (NSView*)any_cast<uintptr_t>(value);
      return true;
    }

    if(name == Video::Synchronize) {
      if(settings.synchronize != any_cast<bool>(value)) {
        settings.synchronize = any_cast<bool>(value);

        if(view) {
          @autoreleasepool {
            [[view openGLContext] makeCurrentContext];
            init();
            OpenGL::set_shader(settings.shader);
          }
        }
      }
      return true;
    }

    if(name == Video::Filter) {
      settings.filter = any_cast<unsigned>(value);
      return true;
    }

    if(name == Video::Shader) {
      settings.shader = any_cast<const char*>(value);

      @autoreleasepool {
        [[view openGLContext] makeCurrentContext];
        OpenGL::set_shader(settings.shader);
        settings.filter = OpenGL::fragmentfilter;
      }

      return true;
    }

    return false;
  }

  bool lock(uint32_t *&data, unsigned &pitch, unsigned width, unsigned height) {
    resize(width, height);
    settings.width = width;
    settings.height = height;
    return OpenGL::lock(data, pitch);
  }

  void unlock() {
  }

  void clear() {
    @autoreleasepool {
      [view lockFocus];
      OpenGL::clear();
      [[view openGLContext] flushBuffer];
      [view unlockFocus];
    }
  }

  void refresh() {
    @autoreleasepool {
      if([view lockFocusIfCanDraw]) {
        auto area = [view frame];
        OpenGL::refresh(
          settings.filter == Video::FilterLinear,
          settings.width, settings.height,
          area.size.width, area.size.height
        );
        [[view openGLContext] flushBuffer];
        [view unlockFocus];
      }
    }
  }

  bool init() {
    term();

    @autoreleasepool {
      NSOpenGLPixelFormatAttribute attributes[] = {
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAWindow,
        0
      };

      auto size = [settings.handle frame].size;
      auto format = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];

      view = [[RubyVideoCGL alloc] initWith:this pixelFormat:format];
      [view setFrame:NSMakeRect(0, 0, size.width, size.height)];
      [view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
      [settings.handle addSubview:view];

      [view lockFocus];

      OpenGL::init();
      settings.width = 256;
      settings.height = 256;

      auto context = (CGLContextObj)[[view openGLContext] CGLContextObj];
      int synchronize = settings.synchronize;
      CGLSetParameter(context, kCGLCPSwapInterval, &synchronize);

      [view unlockFocus];
    }

    clear();
    return true;
  }

  void term() {
    OpenGL::term();

    @autoreleasepool {
      [view removeFromSuperview];
      [view release];
      view = nil;
    }
  }

  pVideoCGL() {
    view = nil;

    settings.handle = nil;
    settings.synchronize = false;
    settings.filter = 0;

    iformat = GL_UNSIGNED_INT_8_8_8_8_REV;
    ibpp = 4;
  }

  ~pVideoCGL() {
    term();
  }
};

DeclareVideo(CGL)

}

@implementation RubyVideoCGL : NSOpenGLView

-(id) initWith:(ruby::pVideoCGL*)videoPointer pixelFormat:(NSOpenGLPixelFormat*)pixelFormat {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0) pixelFormat:pixelFormat]) {
    video = videoPointer;
  }
  return self;
}

-(void) reshape {
  video->refresh();
}

@end
