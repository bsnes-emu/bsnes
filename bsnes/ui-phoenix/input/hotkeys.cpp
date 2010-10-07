void InputMapper::poll_hotkeys(unsigned scancode, int16_t value) {
  if(value == 0) return;
  if(scancode == keyboard(0)[Keyboard::Escape]) input.unacquire();
  if(mainWindow.focused() == false) return;
}
