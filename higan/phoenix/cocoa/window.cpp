@implementation CocoaWindow : NSWindow

-(id) initWith :(phoenix::Window&)windowReference {
  window = &windowReference;

  NSUInteger style = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
  if(window->state.resizable) style |= NSResizableWindowMask;

  if(self = [super initWithContentRect:NSMakeRect(0, 0, 640, 480) styleMask:style backing:NSBackingStoreBuffered defer:YES]) {
    [self setDelegate:self];
    [self setReleasedWhenClosed:NO];
    [self setAcceptsMouseMovedEvents:YES];
    [self setLevel:NSFloatingWindowLevel];  //when launched from a terminal, this places the window above it
    [self setTitle:@""];

    menuBar = [[NSMenu alloc] init];

    NSMenuItem *item;
    string text;

    rootMenu = [[NSMenu alloc] init];
    item = [[[NSMenuItem alloc] initWithTitle:@"" action:nil keyEquivalent:@""] autorelease];
    [item setSubmenu:rootMenu];
    [menuBar addItem:item];

    text = {"About ", phoenix::applicationState.name, " ..."};
    item = [[[NSMenuItem alloc] initWithTitle:[NSString stringWithUTF8String:text] action:@selector(menuAbout) keyEquivalent:@""] autorelease];
    [rootMenu addItem:item];
    [rootMenu addItem:[NSMenuItem separatorItem]];

    item = [[[NSMenuItem alloc] initWithTitle:@"Preferences" action:@selector(menuPreferences) keyEquivalent:@""] autorelease];
    [rootMenu addItem:item];
    [rootMenu addItem:[NSMenuItem separatorItem]];

    text = {"Quit ", phoenix::applicationState.name};
    item = [[[NSMenuItem alloc] initWithTitle:[NSString stringWithUTF8String:text] action:@selector(menuQuit) keyEquivalent:@""] autorelease];
    [rootMenu addItem:item];
  }

  return self;
}

-(BOOL) canBecomeKeyWindow {
  return YES;
}

-(BOOL) canBecomeMainWindow {
  return YES;
}

-(void) windowDidBecomeMain :(NSNotification*)notification {
  if(window->state.menu.size() > 0) {
    [NSApp setMainMenu:menuBar];
  }
}

-(void) windowDidMove :(NSNotification*)notification {
  window->p.moveEvent();
}

-(void) windowDidResize :(NSNotification*)notification {
  window->p.sizeEvent();
}

-(BOOL) windowShouldClose :(id)sender {
  if(window->onClose) window->onClose();
  else window->setVisible(false);
  if(window->state.modal && !window->visible()) window->setModal(false);
  return NO;
}

-(NSMenu*) menuBar {
  return menuBar;
}

-(void) menuAbout {
  using phoenix::Application;
  if(Application::Cocoa::onAbout) Application::Cocoa::onAbout();
}

-(void) menuPreferences {
  using phoenix::Application;
  if(Application::Cocoa::onPreferences) Application::Cocoa::onPreferences();
}

-(void) menuQuit {
  using phoenix::Application;
  if(Application::Cocoa::onQuit) Application::Cocoa::onQuit();
}

@end

namespace phoenix {

Window& pWindow::none() {
  static Window *window = nullptr;
  if(window == nullptr) window = new Window;
  return *window;
}

void pWindow::append(Layout &layout) {
  Geometry geometry = window.state.geometry;
  geometry.x = geometry.y = 0;
  layout.setGeometry(geometry);
}

void pWindow::append(Menu &menu) {
  @autoreleasepool {
    [[cocoaWindow menuBar] addItem:menu.p.cocoaAction];
  }
}

void pWindow::append(Widget &widget) {
  if(widget.font().empty() && !window.state.widgetFont.empty()) {
    widget.setFont(window.state.widgetFont);
  }

  @autoreleasepool {
    [widget.p.cocoaView removeFromSuperview];
    [[cocoaWindow contentView] addSubview:widget.p.cocoaView positioned:NSWindowAbove relativeTo:nil];
    widget.p.setGeometry(widget.geometry());
    [[cocoaWindow contentView] setNeedsDisplay:YES];
  }
}

Color pWindow::backgroundColor() {
  @autoreleasepool {
    NSColor *color = [[cocoaWindow backgroundColor] colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
    return {
      uint8_t(255 * [color redComponent]),
      uint8_t(255 * [color greenComponent]),
      uint8_t(255 * [color blueComponent]),
      uint8_t(255 * [color alphaComponent])
    };
  }
}

bool pWindow::focused() {
  @autoreleasepool {
    return [cocoaWindow isMainWindow] == YES;
  }
}

Geometry pWindow::frameMargin() {
  @autoreleasepool {
    NSRect frame = [cocoaWindow frameRectForContentRect:NSMakeRect(0, 0, 640, 480)];
    return {abs(frame.origin.x), frame.size.height - 480, frame.size.width - 640, abs(frame.origin.y)};
  }
}

Geometry pWindow::geometry() {
  @autoreleasepool {
    NSRect area = [cocoaWindow contentRectForFrameRect:[cocoaWindow frame]];
    return {area.origin.x, Desktop::size().height - area.origin.y - area.size.height, area.size.width, area.size.height};
  }
}

void pWindow::remove(Layout &layout) {
  @autoreleasepool {
    [[cocoaWindow contentView] setNeedsDisplay:YES];
  }
}

void pWindow::remove(Menu &menu) {
  @autoreleasepool {
    [[cocoaWindow menuBar] removeItem:menu.p.cocoaAction];
  }
}

void pWindow::remove(Widget &widget) {
  @autoreleasepool {
    [widget.p.cocoaView removeFromSuperview];
    [[cocoaWindow contentView] setNeedsDisplay:YES];
  }
}

void pWindow::setBackgroundColor(const Color &color) {
  @autoreleasepool {
    [cocoaWindow
      setBackgroundColor:[NSColor
        colorWithCalibratedRed:color.red / 255.0
        green:color.green / 255.0
        blue:color.blue / 255.0
        alpha:color.alpha / 255.0
      ]
    ];
  }
}

void pWindow::setFocused() {
  @autoreleasepool {
    [cocoaWindow makeKeyAndOrderFront:nil];
  }
}

void pWindow::setFullScreen(bool fullScreen) {
  @autoreleasepool {
    if(fullScreen == true) {
      [NSApp setPresentationOptions:NSApplicationPresentationFullScreen];
      [cocoaWindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
      [cocoaWindow toggleFullScreen:nil];
    } else {
      [cocoaWindow toggleFullScreen:nil];
      [cocoaWindow setCollectionBehavior:NSWindowCollectionBehaviorDefault];
      [NSApp setPresentationOptions:NSApplicationPresentationDefault];
    }
  }
}

void pWindow::setGeometry(const Geometry &geometry) {
  locked = true;

  @autoreleasepool {
    [cocoaWindow
      setFrame:[cocoaWindow
        frameRectForContentRect:NSMakeRect(geometry.x, Desktop::size().height - geometry.y - geometry.height, geometry.width, geometry.height)
      ]
      display:YES
    ];

    for(auto &layout : window.state.layout) {
      Geometry geometry = this->geometry();
      geometry.x = geometry.y = 0;
      layout.setGeometry(geometry);
    }
  }

  locked = false;
}

void pWindow::setMenuFont(const string &font) {
}

void pWindow::setMenuVisible(bool visible) {
}

void pWindow::setModal(bool modal) {
  @autoreleasepool {
    if(modal == true) {
      [NSApp runModalForWindow:cocoaWindow];
    } else {
      [NSApp stopModal];
      NSEvent *event = [NSEvent otherEventWithType:NSApplicationDefined location:NSMakePoint(0, 0) modifierFlags:0 timestamp:0.0 windowNumber:0 context:nil subtype:0 data1:0 data2:0];
      [NSApp postEvent:event atStart:true];
    }
  }
}

void pWindow::setResizable(bool resizable) {
  @autoreleasepool {
    NSUInteger style = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
    if(resizable) style |= NSResizableWindowMask;
    [cocoaWindow setStyleMask:style];
  }
}

void pWindow::setStatusFont(const string &font) {
}

void pWindow::setStatusText(const string &text) {
}

void pWindow::setStatusVisible(bool visible) {
}

void pWindow::setTitle(const string &text) {
  @autoreleasepool {
    [cocoaWindow setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pWindow::setVisible(bool visible) {
  @autoreleasepool {
    if(visible) {
      [cocoaWindow makeKeyAndOrderFront:nil];
      [cocoaWindow setLevel:NSNormalWindowLevel];
    } else {
      [cocoaWindow orderOut:nil];
    }
  }
}

void pWindow::setWidgetFont(const string &font) {
}

void pWindow::constructor() {
  @autoreleasepool {
    cocoaWindow = [[CocoaWindow alloc] initWith:window];
  }
}

void pWindow::destructor() {
  @autoreleasepool {
    [cocoaWindow release];
  }
}

void pWindow::moveEvent() {
  if(locked == false && window.fullScreen() == false && window.visible() == true) {
    Geometry geometry = this->geometry();
    window.state.geometry.x = geometry.x;
    window.state.geometry.y = geometry.y;
  }

  if(locked == false) {
    if(window.onMove) window.onMove();
  }
}

void pWindow::sizeEvent() {
  if(locked == false && window.fullScreen() == false && window.visible() == true) {
    Geometry geometry = this->geometry();
    window.state.geometry.width = geometry.width;
    window.state.geometry.height = geometry.height;
  }

  for(auto &layout : window.state.layout) {
    Geometry geometry = this->geometry();
    geometry.x = geometry.y = 0;
    layout.setGeometry(geometry);
  }

  if(locked == false) {
    if(window.onSize) window.onSize();
  }
}

}
