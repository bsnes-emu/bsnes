namespace phoenix {

MessageWindow::Response pMessageWindow::information(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return message(parent, text, buttons, Type::Information);
}

MessageWindow::Response pMessageWindow::question(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return message(parent, text, buttons, Type::Question);
}

MessageWindow::Response pMessageWindow::warning(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return message(parent, text, buttons, Type::Warning);
}

MessageWindow::Response pMessageWindow::critical(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return message(parent, text, buttons, Type::Critical);
}

MessageWindow::Response pMessageWindow::message(Window &parent, const string &text, MessageWindow::Buttons buttons, Type type) {
  @autoreleasepool {
    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert setMessageText:[NSString stringWithUTF8String:text]];

    switch(buttons) {
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
    case Type::Information: [alert setAlertStyle:NSInformationalAlertStyle]; break;
    case Type::Question: [alert setAlertStyle:NSInformationalAlertStyle]; break;
    case Type::Warning: [alert setAlertStyle:NSWarningAlertStyle]; break;
    case Type::Critical: [alert setAlertStyle:NSCriticalAlertStyle]; break;
    }

    NSInteger response = [alert runModal];
  //[alert beginSheetModalForWindow:parent.p.cocoaWindow modalDelegate:self didEndSelector:@selector(...) contextInfo:nil];

    switch(buttons) {
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

  return MessageWindow::Response::Ok;
}

}
