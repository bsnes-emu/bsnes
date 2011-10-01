#include "mmc1.cpp"
#include "mmc3.cpp"
#include "vrc6.cpp"

void Chip::tick() {
  board.tick();
}

Chip::Chip(Board &board) : board(board) {
}
