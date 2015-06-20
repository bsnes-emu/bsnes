#include "opengl/opengl.hpp"

struct VideoCGL;

@interface RubyVideoCGL : NSOpenGLView {
@public
  ruby::VideoCGL* video;
}
-(id) initWith:(ruby::VideoCGL*)video pixelFormat:(NSOpenGLPixelFormat*)pixelFormat;
-(void) reshape;
@end

struct VideoCGL : Video, OpenGL {
  ~VideoCGL() { term(); }

  RubyVideoCGL* view = nullptr;

  struct {
    NSView* handle = nullptr;
    bool synchronize = false;
    unsigned filter = Video::FilterNearest;
    string shader;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Video::Handle) return true;
    if(name == Video::Synchronize) return true;
    if(name == Video::Filter) return true;
    if(name == Video::Shader) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    if(name == Video::Synchronize) return settings.synchronize;
    if(name == Video::Filter) return settings.filter;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Video::Handle && value.is<uintptr_t>()) {
      settings.handle = (NSView*)value.get<uintptr_t>();
      return true;
    }

    if(name == Video::Synchronize && value.is<bool>()) {
      if(settings.synchronize != value.get<bool>()) {
        settings.synchronize = value.get<bool>();

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

    if(name == Video::Filter && value.is<unsigned>()) {
      settings.filter = value.get<unsigned>();
      if(!settings.shader) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
      return true;
    }

    if(name == Video::Shader && value.is<string>()) {
      settings.shader = value.get<string>();
      @autoreleasepool {
        [[view openGLContext] makeCurrentContext];
      }
      OpenGL::shader(settings.shader);
      if(!settings.shader) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
      return true;
    }

    return false;
  }

  auto lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) -> bool {
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  auto unlock() -> void {
  }

  auto clear() -> void {
    @autoreleasepool {
      [view lockFocus];
      OpenGL::clear();
      [[view openGLContext] flushBuffer];
      [view unlockFocus];
    }
  }

  auto refresh() -> void {
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

  auto init() -> bool {
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

  auto term() -> void {
    OpenGL::term();

    @autoreleasepool {
      [view removeFromSuperview];
      [view release];
      view = nil;
    }
  }
};

@implementation RubyVideoCGL : NSOpenGLView

-(id) initWith:(ruby::VideoCGL*)videoPointer pixelFormat:(NSOpenGLPixelFormat*)pixelFormat {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0) pixelFormat:pixelFormat]) {
    video = videoPointer;
  }
  return self;
}

-(void) reshape {
  video->refresh();
}

@end
