#define GL_ALPHA_TEST 0x0bc0
#include "opengl/opengl.hpp"

struct VideoCGL;

@interface RubyVideoCGL : NSOpenGLView {
@public
  VideoCGL* video;
}
-(id) initWith:(VideoCGL*)video pixelFormat:(NSOpenGLPixelFormat*)pixelFormat;
-(void) reshape;
-(BOOL) acceptsFirstResponder;
@end

@interface RubyWindowCGL : NSWindow <NSWindowDelegate> {
@public
  VideoCGL* video;
}
-(id) initWith:(VideoCGL*)video;
-(BOOL) canBecomeKeyWindow;
-(BOOL) canBecomeMainWindow;
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

  auto hasFullScreen() -> bool override { return true; }
  auto hasContext() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }
  auto hasFlush() -> bool override { return true; }
  auto hasShader() -> bool override { return true; }

  auto setFullScreen(bool fullScreen) -> bool override {
    return initialize();
  }

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

  auto size(uint& width, uint& height) -> void override {
    @autoreleasepool {
      auto area = [view convertRectToBacking:[view bounds]];
      width = area.size.width;
      height = area.size.height;
    }
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  auto release() -> void override {
  }

  auto output(uint width, uint height) -> void override {
    uint windowWidth, windowHeight;
    size(windowWidth, windowHeight);

    @autoreleasepool {
      if([view lockFocusIfCanDraw]) {
        OpenGL::absoluteWidth = width;
        OpenGL::absoluteHeight = height;
        OpenGL::outputX = 0;
        OpenGL::outputY = 0;
        OpenGL::outputWidth = windowWidth;
        OpenGL::outputHeight = windowHeight;
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
    if(!self.fullScreen && !self.context) return false;

    @autoreleasepool {
      if(self.fullScreen) {
        window = [[RubyWindowCGL alloc] initWith:this];
        [window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
        [window toggleFullScreen:nil];
      //[NSApp setPresentationOptions:NSApplicationPresentationFullScreen];
      }

      NSOpenGLPixelFormatAttribute attributeList[] = {
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADoubleBuffer,
        0
      };

      auto context = self.fullScreen ? [window contentView] : (NSView*)self.context;
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
      [openGLContext makeCurrentContext];
      [[view window] makeFirstResponder:view];
      [view lockFocus];

      OpenGL::initialize(self.shader);

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

    @autoreleasepool {
      if(view) {
        [view removeFromSuperview];
        [view release];
        view = nil;
      }

      if(window) {
      //[NSApp setPresentationOptions:NSApplicationPresentationDefault];
        [window toggleFullScreen:nil];
        [window setCollectionBehavior:NSWindowCollectionBehaviorDefault];
        [window close];
        [window release];
        window = nil;
      }
    }
  }

  RubyVideoCGL* view = nullptr;
  RubyWindowCGL* window = nullptr;

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
  video->output(0, 0);
}

-(BOOL) acceptsFirstResponder {
  return YES;
}

-(void) keyDown:(NSEvent*)event {
}

-(void) keyUp:(NSEvent*)event {
}

@end

@implementation RubyWindowCGL : NSWindow

-(id) initWith:(VideoCGL*)videoPointer {
  auto primaryRect = [[[NSScreen screens] objectAtIndex:0] frame];
  if(self = [super initWithContentRect:primaryRect styleMask:0 backing:NSBackingStoreBuffered defer:YES]) {
    video = videoPointer;
    [self setDelegate:self];
    [self setReleasedWhenClosed:NO];
    [self setAcceptsMouseMovedEvents:YES];
    [self setTitle:@""];
    [self makeKeyAndOrderFront:nil];
  }
  return self;
}

-(BOOL) canBecomeKeyWindow {
  return YES;
}

-(BOOL) canBecomeMainWindow {
  return YES;
}

@end
