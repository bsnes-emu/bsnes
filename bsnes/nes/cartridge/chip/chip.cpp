#include "mmc1.cpp"
#include "mmc3.cpp"
#include "mmc5.cpp"
#include "vrc4.cpp"
#include "vrc6.cpp"
#include "vrc7.cpp"

void Chip::tick() {
  board.tick();
}

Chip::Chip(Board &board) : board(board) {
}
