#include "opengl/opengl.hpp"

namespace ruby {
  class pVideoCGL;
}

@interface RubyVideoCGL : NSOpenGLView {
@public
  ruby::pVideoCGL* video;
}
-(id) initWith:(ruby::pVideoCGL*)video pixelFormat:(NSOpenGLPixelFormat*)pixelFormat;
-(void) reshape;
@end

namespace ruby {

struct pVideoCGL : OpenGL {
  RubyVideoCGL* view = nullptr;

  struct {
    NSView* handle = nullptr;
    bool synchronize = false;
    unsigned filter = 0;
    string shader;
  } settings;

  bool cap(const string& name) {
    if(name == Video::Handle) return true;
    if(name == Video::Synchronize) return true;
    if(name == Video::Filter) return true;
    if(name == Video::Shader) return true;
    return false;
  }

  any get(const string& name) {
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    if(name == Video::Synchronize) return settings.synchronize;
    if(name == Video::Filter) return settings.filter;
    return false;
  }

  bool set(const string& name, const any& value) {
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
            int synchronize = settings.synchronize;
            [[view openGLContext] setValues:&synchronize forParameter:NSOpenGLCPSwapInterval];
          }
        }
      }
      return true;
    }

    if(name == Video::Filter) {
      settings.filter = any_cast<unsigned>(value);
      if(settings.shader.empty()) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
      return true;
    }

    if(name == Video::Shader) {
      settings.shader = any_cast<const char*>(value);
      @autoreleasepool {
        [[view openGLContext] makeCurrentContext];
      }
      OpenGL::shader(settings.shader);
      if(settings.shader.empty()) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
      return true;
    }

    return false;
  }

  bool lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) {
    OpenGL::size(width, height);
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
        outputWidth = area.size.width, outputHeight = area.size.height;
        OpenGL::refresh();
        [[view openGLContext] flushBuffer];
        [view unlockFocus];
      }
    }
  }

  bool init() {
    term();

    @autoreleasepool {
      NSOpenGLPixelFormatAttribute attributes[] = {
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADoubleBuffer,
        0
      };

      auto size = [settings.handle frame].size;
      auto format = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
      auto context = [[[NSOpenGLContext alloc] initWithFormat:format shareContext:nil] autorelease];

      view = [[RubyVideoCGL alloc] initWith:this pixelFormat:format];
      [view setOpenGLContext:context];
      [view setFrame:NSMakeRect(0, 0, size.width, size.height)];
      [view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
      [settings.handle addSubview:view];
      [context setView:view];

      [view lockFocus];

      OpenGL::init();
    //print((const char*)glGetString(GL_VERSION), "\n");

      int synchronize = settings.synchronize;
      [[view openGLContext] setValues:&synchronize forParameter:NSOpenGLCPSwapInterval];

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
