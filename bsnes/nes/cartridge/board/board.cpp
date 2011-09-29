#include "nrom.cpp"

unsigned Board::mirror(unsigned addr, unsigned size) const {
  unsigned base = 0;
  if(size) {
    unsigned mask = 1 << 23;
    while(addr >= size) {
      while(!(addr & mask)) mask >>= 1;
      addr -= mask;
      if(size > mask) {
        size -= mask;
        base += mask;
      }
      mask >>= 1;
    }
    base += addr;
  }
  return base;
}

uint8 Board::prg_read(unsigned addr) {
  return prg.data[mirror(addr, prg.size)];
}

void Board::prg_write(unsigned addr, uint8 data) {
  prg.data[mirror(addr, prg.size)] = data;
}

uint8 Board::chr_read(unsigned addr) {
  return chr.data[mirror(addr, chr.size)];
}

void Board::chr_write(unsigned addr, uint8 data) {
  chr.data[mirror(addr, chr.size)] = data;
}

Board* Board::create(const string &xml, const uint8_t *data, unsigned size) {
  string type;
  string configuration;

  xml_element document = xml_parse(xml);
  for(auto &head : document.element) {
    if(head.name == "cartridge") {
      for(auto &node : head.element) {
        if(node.name == "board") {
          configuration = node.content;
          for(auto &attr : node.attribute) {
            if(attr.name == "type") type = attr.parse();
          }
        }
      }
    }
  }

  Board *board = nullptr;
  if(type == "NES-NROM-256") board = new NROM;
  assert(board != nullptr);

  for(auto &head : document.element) {
    if(head.name == "cartridge") {
      for(auto &node : head.element) {
        if(node.name == "board") {
          for(auto &leaf : node.element) {
            if(leaf.name == "prg") {
              for(auto &attr : leaf.attribute) {
                if(attr.name == "size") board->prg.size = decimal(attr.content);
              }
            }

            if(leaf.name == "chr") {
              for(auto &attr : leaf.attribute) {
                if(attr.name == "size") board->chr.size = decimal(attr.content);
              }
            }
          }
        }
      }
    }
  }

  board->prg.data = new uint8[board->prg.size];
  memcpy(board->prg.data, data, board->prg.size);

  board->chr.data = new uint8[board->chr.size];
  memcpy(board->chr.data, data + board->prg.size, board->chr.size);

  board->configure({ "<board>\n", configuration, "</board>\n" });

  return board;
}
