namespace phoenix {

string pBrowserWindow::directory(BrowserWindow::State& state) {
  string result;

  @autoreleasepool {
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    if(state.title) [panel setTitle:[NSString stringWithUTF8String:state.title]];
    [panel setCanChooseDirectories:YES];
    [panel setCanChooseFiles:NO];
    if([panel runModalForDirectory:[NSString stringWithUTF8String:state.path] file:nil] == NSOKButton) {
      NSArray* names = [panel filenames];
      const char* name = [[names objectAtIndex:0] UTF8String];
      if(name) result = name;
    }
  }

  return result;
}

string pBrowserWindow::open(BrowserWindow::State& state) {
  string result;

  @autoreleasepool {
    NSMutableArray* filters = [[NSMutableArray alloc] init];
    for(auto& rule : state.filters) {
      string pattern = rule.split<1>("(")(1).rtrim(")");
      if(!pattern.empty()) [filters addObject:[NSString stringWithUTF8String:pattern]];
    }
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    if(state.title) [panel setTitle:[NSString stringWithUTF8String:state.title]];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setAllowedFileTypes:filters];
    if([panel runModalForDirectory:[NSString stringWithUTF8String:state.path] file:nil] == NSOKButton) {
      NSArray* names = [panel filenames];
      const char* name = [[names objectAtIndex:0] UTF8String];
      if(name) result = name;
    }
    [filters release];
  }

  return result;
}

string pBrowserWindow::save(BrowserWindow::State& state) {
  string result;

  @autoreleasepool {
    NSMutableArray* filters = [[NSMutableArray alloc] init];
    for(auto& rule : state.filters) {
      string pattern = rule.split<1>("(")(1).rtrim(")");
      if(!pattern.empty()) [filters addObject:[NSString stringWithUTF8String:pattern]];
    }
    NSSavePanel* panel = [NSSavePanel savePanel];
    if(state.title) [panel setTitle:[NSString stringWithUTF8String:state.title]];
    [panel setAllowedFileTypes:filters];
    if([panel runModalForDirectory:[NSString stringWithUTF8String:state.path] file:nil] == NSOKButton) {
      NSArray* names = [panel filenames];
      const char* name = [[names objectAtIndex:0] UTF8String];
      if(name) result = name;
    }
    [filters release];
  }

  return result;
}

}
