namespace phoenix {

string pDialogWindow::fileOpen(Window &parent, const string &path, const lstring &filter) {
  string result;

  @autoreleasepool {
    NSMutableArray *filters = [[NSMutableArray alloc] init];
    for(auto &rule : filter) {
      string pattern = rule.split<1>("(")(1).rtrim<1>(")");
      if(!pattern.empty()) [filters addObject:[NSString stringWithUTF8String:pattern]];
    }
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setAllowedFileTypes:filters];
    if([panel runModalForDirectory:[NSString stringWithUTF8String:path] file:nil] == NSOKButton) {
      NSArray *filenames = [panel filenames];
      const char *filename = [[filenames objectAtIndex:0] UTF8String];
      if(filename) result = filename;
    }
    [filters release];
  }

  return result;
}

string pDialogWindow::fileSave(Window &parent, const string &path, const lstring &filter) {
  string result;

  @autoreleasepool {
    NSMutableArray *filters = [[NSMutableArray alloc] init];
    for(auto &rule : filter) {
      string pattern = rule.split<1>("(")(1).rtrim<1>(")");
      if(!pattern.empty()) [filters addObjects:[NSString stringWithUTF8String:pattern]];
    }
    NSSavePanel *panel = [NSSavePanel savePanel];
    [panel setAllowedFileTypes:filters];
    if([panel runModalForDirectory:[NSString stringWithUTF8String:path] file:nil] == NSOKButton) {
      NSArray *filenames = [panel filenames];
      const char *filename = [[filenames objectAtIndex:0] UTF8String];
      if(filename) result = filename;
    }
    [filters release];
  }

  return result;
}

string pDialogWindow::folderSelect(Window &parent, const string &path) {
  string result;

  @autoreleasepool {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setCanChooseDirectories:YES];
    [panel setCanChooseFiles:NO];
    if([panel runModalForDirectory:[NSString stringWithUTF8String:path] file:nil] == NSOKButton) {
      NSArray *filenames = [panel filenames];
      const char *filename = [[filenames objectAtIndex:0] UTF8String];
      if(filename) result = filename;
    }
  }

  return result;
}

}
