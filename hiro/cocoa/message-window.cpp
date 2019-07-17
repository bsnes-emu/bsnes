#if defined(Hiro_MessageWindow)

namespace hiro {

enum class MessageWindowType : uint { Error, Information, Question, Warning };

auto MessageWindow_dialog(MessageWindow::State& state, MessageWindowType type) -> MessageWindow::Response {
  @autoreleasepool {
    NSAlert* alert = [[[NSAlert alloc] init] autorelease];
    if(state.title) [alert setMessageText:[NSString stringWithUTF8String:state.title]];
    [alert setInformativeText:[NSString stringWithUTF8String:state.text]];

    switch(state.buttons) {
    case MessageWindow::Buttons::Ok:
      [alert addButtonWithTitle:@"Ok"];
      break;
    case MessageWindow::Buttons::OkCancel:
      [alert addButtonWithTitle:@"Ok"];
      [alert addButtonWithTitle:@"Cancel"];
      break;
    case MessageWindow::Buttons::YesNo:
      [alert addButtonWithTitle:@"Yes"];
      [alert addButtonWithTitle:@"No"];
      break;
    case MessageWindow::Buttons::YesNoCancel:
      [alert addButtonWithTitle:@"Yes"];
      [alert addButtonWithTitle:@"No"];
      [alert addButtonWithTitle:@"Cancel"];
      break;
    }

    switch(type) {
    case MessageWindowType::Error: [alert setAlertStyle:NSCriticalAlertStyle]; break;
    case MessageWindowType::Information: [alert setAlertStyle:NSInformationalAlertStyle]; break;
    case MessageWindowType::Question: [alert setAlertStyle:NSInformationalAlertStyle]; break;
    case MessageWindowType::Warning: [alert setAlertStyle:NSWarningAlertStyle]; break;
    }

    NSInteger response = [alert runModal];
  //[alert beginSheetModalForWindow:parent.p.cocoaWindow modalDelegate:self didEndSelector:@selector(...) contextInfo:nil];

    switch(state.buttons) {
    case MessageWindow::Buttons::Ok:
      if(response == NSAlertFirstButtonReturn) return MessageWindow::Response::Ok;
      break;
    case MessageWindow::Buttons::OkCancel:
      if(response == NSAlertFirstButtonReturn) return MessageWindow::Response::Ok;
      if(response == NSAlertSecondButtonReturn) return MessageWindow::Response::Cancel;
      break;
    case MessageWindow::Buttons::YesNo:
      if(response == NSAlertFirstButtonReturn) return MessageWindow::Response::Yes;
      if(response == NSAlertSecondButtonReturn) return MessageWindow::Response::No;
      break;
    case MessageWindow::Buttons::YesNoCancel:
      if(response == NSAlertFirstButtonReturn) return MessageWindow::Response::Yes;
      if(response == NSAlertSecondButtonReturn) return MessageWindow::Response::No;
      if(response == NSAlertThirdButtonReturn) return MessageWindow::Response::Cancel;
      break;
    }
  }

  throw;
}

auto pMessageWindow::error(MessageWindow::State& state) -> MessageWindow::Response {
  return MessageWindow_dialog(state, MessageWindowType::Error);
}

auto pMessageWindow::information(MessageWindow::State& state) -> MessageWindow::Response {
  return MessageWindow_dialog(state, MessageWindowType::Information);
}

auto pMessageWindow::question(MessageWindow::State& state) -> MessageWindow::Response {
  return MessageWindow_dialog(state, MessageWindowType::Question);
}

auto pMessageWindow::warning(MessageWindow::State& state) -> MessageWindow::Response {
  return MessageWindow_dialog(state, MessageWindowType::Warning);
}

}

#endif
