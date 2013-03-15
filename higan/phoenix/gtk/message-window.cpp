namespace phoenix {

static MessageWindow::Response MessageWindow_response(MessageWindow::Buttons buttons, gint response) {
  if(response == GTK_RESPONSE_OK) return MessageWindow::Response::Ok;
  if(response == GTK_RESPONSE_CANCEL) return MessageWindow::Response::Cancel;
  if(response == GTK_RESPONSE_YES) return MessageWindow::Response::Yes;
  if(response == GTK_RESPONSE_NO) return MessageWindow::Response::No;
  if(buttons == MessageWindow::Buttons::OkCancel) return MessageWindow::Response::Cancel;
  if(buttons == MessageWindow::Buttons::YesNo) return MessageWindow::Response::No;
  return MessageWindow::Response::Ok;
}

MessageWindow::Response pMessageWindow::information(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  GtkButtonsType buttonsType = GTK_BUTTONS_OK;
  if(buttons == MessageWindow::Buttons::OkCancel) buttonsType = GTK_BUTTONS_OK_CANCEL;
  if(buttons == MessageWindow::Buttons::YesNo) buttonsType = GTK_BUTTONS_YES_NO;
  GtkWidget *dialog = gtk_message_dialog_new(
    &parent != &Window::none() ? GTK_WINDOW(parent.p.widget) : (GtkWindow*)nullptr,
    GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, buttonsType, "%s", (const char*)text
  );
  gint response = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  return MessageWindow_response(buttons, response);
}

MessageWindow::Response pMessageWindow::question(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  GtkButtonsType buttonsType = GTK_BUTTONS_OK;
  if(buttons == MessageWindow::Buttons::OkCancel) buttonsType = GTK_BUTTONS_OK_CANCEL;
  if(buttons == MessageWindow::Buttons::YesNo) buttonsType = GTK_BUTTONS_YES_NO;
  GtkWidget *dialog = gtk_message_dialog_new(
    &parent != &Window::none() ? GTK_WINDOW(parent.p.widget) : (GtkWindow*)nullptr,
    GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, buttonsType, "%s", (const char*)text
  );
  gint response = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  return MessageWindow_response(buttons, response);
}

MessageWindow::Response pMessageWindow::warning(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  GtkButtonsType buttonsType = GTK_BUTTONS_OK;
  if(buttons == MessageWindow::Buttons::OkCancel) buttonsType = GTK_BUTTONS_OK_CANCEL;
  if(buttons == MessageWindow::Buttons::YesNo) buttonsType = GTK_BUTTONS_YES_NO;
  GtkWidget *dialog = gtk_message_dialog_new(
    &parent != &Window::none() ? GTK_WINDOW(parent.p.widget) : (GtkWindow*)nullptr,
    GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, buttonsType, "%s", (const char*)text
  );
  gint response = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  return MessageWindow_response(buttons, response);
}

MessageWindow::Response pMessageWindow::critical(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  GtkButtonsType buttonsType = GTK_BUTTONS_OK;
  if(buttons == MessageWindow::Buttons::OkCancel) buttonsType = GTK_BUTTONS_OK_CANCEL;
  if(buttons == MessageWindow::Buttons::YesNo) buttonsType = GTK_BUTTONS_YES_NO;
  GtkWidget *dialog = gtk_message_dialog_new(
    &parent != &Window::none() ? GTK_WINDOW(parent.p.widget) : (GtkWindow*)nullptr,
    GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, buttonsType, "%s", (const char*)text
  );
  gint response = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  return MessageWindow_response(buttons, response);
}

}
