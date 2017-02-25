auto PSG::write(uint8 data) -> void {
  bool l = data.bit(7);
  if(l) select = data.bits(4,6);

  switch(select) {

  case 0: {
    if(l) tone0.pitch.bits(0,3) = data.bits(0,3);
    else  tone0.pitch.bits(4,9) = data.bits(0,5);
    break;
  }

  case 1: {
    tone0.volume = data.bits(0,3);
    break;
  }

  case 2: {
    if(l) tone1.pitch.bits(0,3) = data.bits(0,3);
    else  tone1.pitch.bits(4,9) = data.bits(0,5);
    break;
  }

  case 3: {
    tone1.volume = data.bits(0,3);
    break;
  }

  case 4: {
    if(l) tone2.pitch.bits(0,3) = data.bits(0,3);
    else  tone2.pitch.bits(4,9) = data.bits(0,5);
    noise.pitch = tone2.pitch;
    break;
  }

  case 5: {
    tone2.volume = data.bits(0,3);
    break;
  }

  case 6: {
    noise.rate = data.bits(0,1);
    noise.enable = data.bit(2);
    noise.lfsr = 0x8000;
    break;
  }

  case 7: {
    noise.volume = data.bits(0,3);
    break;
  }

  }
}

//Game Gear only
auto PSG::balance(uint8 data) -> void {
  tone0.right = data.bit(0);
  tone1.right = data.bit(1);
  tone2.right = data.bit(2);
  noise.right = data.bit(3);
  tone0.left = data.bit(4);
  tone1.left = data.bit(5);
  tone2.left = data.bit(6);
  noise.left = data.bit(7);
}
