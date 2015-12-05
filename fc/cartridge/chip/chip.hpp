struct Board;

struct Chip {
  Chip(Board& board);
  auto tick() -> void;

  Board& board;
};
