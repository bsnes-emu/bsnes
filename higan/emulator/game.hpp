#pragma once

namespace Emulator {

struct Game {
  struct Memory;
  struct Oscillator;

  inline auto load(string_view) -> void;
  inline auto memory(string_view) -> maybe<Memory>;
  inline auto oscillator(natural = 0) -> maybe<Oscillator>;

  struct Memory {
    explicit operator bool() const { return type; }
    inline auto name() const -> string;

    string type;
    boolean battery;
    natural size;
    string category;
    string manufacturer;
    string part;
  };

  struct Oscillator {
    explicit operator bool() const { return frequency; }

    natural frequency;
  };

  Markup::Node document;
  string sha256;
  string label;
  string name;
  string region;
  string revision;
  string board;
  vector<Memory> memoryList;
  vector<Oscillator> oscillatorList;
};

auto Game::load(string_view text) -> void {
  document = BML::unserialize(text);

  sha256 = document["game/sha256"].text();
  label = document["game/label"].text();
  name = document["game/name"].text();
  region = document["game/region"].text();
  revision = document["game/revision"].text();
  board = document["game/board"].text();

  for(auto node : document.find("game/board/memory")) {
    Memory memory;
    memory.type = node["type"].text();
    memory.battery = (bool)node["type/battery"];
    memory.size = node["size"].natural();
    memory.category = node["category"].text();
    memory.manufacturer = node["manufacturer"].text();
    memory.part = node["part"].text();
    memoryList.append(memory);
  }

  for(auto node : document.find("game/board/oscillator")) {
    Oscillator oscillator;
    oscillator.frequency = node["frequency"].natural();
    oscillatorList.append(oscillator);
  }
}

auto Game::memory(string_view name) -> maybe<Memory> {
  for(auto& memory : memoryList) {
    if(memory.name() == name) return memory;
  }
  return nothing;
}

auto Game::oscillator(natural index) -> maybe<Oscillator> {
  if(index < oscillatorList.size()) return oscillatorList[index];
  return nothing;
}

auto Game::Memory::name() const -> string {
  if(part) return string{part, ".", category, ".", type}.downcase();
  return string{category, ".", type}.downcase();
}

}
