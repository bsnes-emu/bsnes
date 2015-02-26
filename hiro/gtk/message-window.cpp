namespace hiro {

static auto Message(MessageWindow::State& state, GtkMessageType messageStyle) -> MessageWindow::Response {
  GtkWidget* dialog = gtk_message_dialog_new(
    state.parent && state.parent->self() ? GTK_WINDOW(state.parent->self()->widget) : (GtkWindow*)nullptr,
    GTK_DIALOG_MODAL, messageStyle, GTK_BUTTONS_NONE, "%s", (const char*)state.text
  );

  if(state.title) gtk_window_set_title(GTK_WINDOW(dialog), state.title);
  else if(Application::state.name) gtk_window_set_title(GTK_WINDOW(dialog), Application::state.name);

  switch(state.buttons) {
  case MessageWindow::Buttons::Ok:
    gtk_dialog_add_buttons(GTK_DIALOG(dialog), "Ok", GTK_RESPONSE_OK, nullptr);
    break;
  case MessageWindow::Buttons::OkCancel:
    gtk_dialog_add_buttons(GTK_DIALOG(dialog), "Ok", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, nullptr);
    break;
  case MessageWindow::Buttons::YesNo:
    gtk_dialog_add_buttons(GTK_DIALOG(dialog), "Yes", GTK_RESPONSE_YES, "No", GTK_RESPONSE_NO, nullptr);
    break;
  case MessageWindow::Buttons::YesNoCancel:
    gtk_dialog_add_buttons(GTK_DIALOG(dialog), "Yes", GTK_RESPONSE_YES, "No", GTK_RESPONSE_NO, "Cancel", GTK_RESPONSE_CANCEL, nullptr);
    break;
  }

  auto response = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

  if(response == GTK_RESPONSE_OK) return MessageWindow::Response::Ok;
  if(response == GTK_RESPONSE_CANCEL) return MessageWindow::Response::Cancel;
  if(response == GTK_RESPONSE_YES) return MessageWindow::Response::Yes;
  if(response == GTK_RESPONSE_NO) return MessageWindow::Response::No;

  //if dialog was closed without choosing a button, choose the most appropriate response
  if(state.buttons == MessageWindow::Buttons::Ok) return MessageWindow::Response::Ok;
  if(state.buttons == MessageWindow::Buttons::OkCancel) return MessageWindow::Response::Cancel;
  if(state.buttons == MessageWindow::Buttons::YesNo) return MessageWindow::Response::No;
  if(state.buttons == MessageWindow::Buttons::YesNoCancel) return MessageWindow::Response::Cancel;

  throw;
}

auto pMessageWindow::error(MessageWindow::State& state) -> MessageWindow::Response {
  return Message(state, GTK_MESSAGE_ERROR);
}

auto pMessageWindow::information(MessageWindow::State& state) -> MessageWindow::Response {
  return Message(state, GTK_MESSAGE_INFO);
}

auto pMessageWindow::question(MessageWindow::State& state) -> MessageWindow::Response {
  return Message(state, GTK_MESSAGE_QUESTION);
}

auto pMessageWindow::warning(MessageWindow::State& state) -> MessageWindow::Response {
  return Message(state, GTK_MESSAGE_WARNING);
}

}
