void Player::serialize(serializer& s) {
  s.integer(status.enable);
  s.integer(status.rumble);

  s.integer(status.logoDetected);
  s.integer(status.logoCounter);

  s.integer(status.packet);
  s.integer(status.send);
  s.integer(status.recv);
}
