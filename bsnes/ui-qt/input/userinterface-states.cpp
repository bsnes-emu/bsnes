InputGroup userInterfaceStates(InputCategory::UserInterface, "States");

namespace UserInterfaceStates {

struct Rewind : HotkeyInput {
  void pressed() {
    ::state.rewind();
  }

  Rewind() : HotkeyInput("Rewind", "input.userInterface.states.rewind") {
    name = "KB0::Backspace";
    userInterfaceStates.attach(this);
  }
} rewind;

struct LoadActiveState : HotkeyInput {
  void pressed() {
    ::state.load(::state.active);
  }

  LoadActiveState() : HotkeyInput("Load Active Quick State", "input.userInterface.states.loadActiveQuickState") {
    name = "KB0::F4";
    userInterfaceStates.attach(this);
  }
} loadActiveState;

struct SaveActiveState : HotkeyInput {
  void pressed() {
    ::state.save(::state.active);
  }

  SaveActiveState() : HotkeyInput("Save Active Quick State", "input.userInterface.states.saveActiveQuickState") {
    name = "KB0::F2";
    userInterfaceStates.attach(this);
  }
} saveActiveState;

struct SelectActiveState : HotkeyInput {
  void pressed() {
    stateSelectWindow->show();
  }

  SelectActiveState() : HotkeyInput("Select Active Quick State", "input.userInterface.states.selectActiveQuicKState") {
    name = "KB0::F3";
    userInterfaceStates.attach(this);
  }
} selectActiveState;

struct DecrementAndLoadState : HotkeyInput {
  void pressed() {
    ::state.active = (::state.active + 10 - 1) % 10;
    ::state.load(::state.active);
  }

  DecrementAndLoadState() : HotkeyInput("Decrement and Load State", "input.userInterface.states.decrementAndLoadState") {
    userInterfaceStates.attach(this);
  }
} decrementAndLoadState;

struct SaveAndIncrementState : HotkeyInput {
  void pressed() {
    ::state.save(::state.active);
    ::state.active = (::state.active + 10 + 1) % 10;
  }

  SaveAndIncrementState() : HotkeyInput("Save and Increment State", "input.userInterface.states.saveAndIncrementState") {
    userInterfaceStates.attach(this);
  }
} saveAndIncrementState;

struct DecrementActiveState : HotkeyInput {
  void pressed() {
    ::state.active = (::state.active + 10 - 1) % 10;
    utility.showMessage(string() << "Quick state " << (::state.active + 1) << " selected.");
  }

  DecrementActiveState() : HotkeyInput("Decrement Active Quick State Slot", "input.userInterface.states.decrementActiveQuickState") {
    userInterfaceStates.attach(this);
  }
} decrementActiveState;

struct IncrementActiveState : HotkeyInput {
  void pressed() {
    ::state.active = (::state.active + 10 + 1) % 10;
    utility.showMessage(string() << "Quick state " << (::state.active + 1) << " selected.");
  }

  IncrementActiveState() : HotkeyInput("Increment Active Quick State Slot", "input.userInterface.states.incrementActiveQuickState") {
    userInterfaceStates.attach(this);
  }
} incrementActiveState;

struct LoadState1 : HotkeyInput {
  void pressed() {
    ::state.load(0);
  }

  LoadState1() : HotkeyInput("Load Quick State 1", "input.userInterface.states.loadQuickState1") {
    userInterfaceStates.attach(this);
  }
} loadState1;

struct LoadState2 : HotkeyInput {
  void pressed() {
    ::state.load(1);
  }

  LoadState2() : HotkeyInput("Load Quick State 2", "input.userInterface.states.loadQuickState2") {
    userInterfaceStates.attach(this);
  }
} loadState2;

struct LoadState3 : HotkeyInput {
  void pressed() {
    ::state.load(2);
  }

  LoadState3() : HotkeyInput("Load Quick State 3", "input.userInterface.states.loadQuickState3") {
    userInterfaceStates.attach(this);
  }
} loadState3;

struct SaveState1 : HotkeyInput {
  void pressed() {
    ::state.save(0);
  }

  SaveState1() : HotkeyInput("Save Quick State 1", "input.userInterface.states.saveQuickState1") {
    userInterfaceStates.attach(this);
  }
} saveState1;

struct SaveState2 : HotkeyInput {
  void pressed() {
    ::state.save(1);
  }

  SaveState2() : HotkeyInput("Save Quick State 2", "input.userInterface.states.saveQuickState2") {
    userInterfaceStates.attach(this);
  }
} saveState2;

struct SaveState3 : HotkeyInput {
  void pressed() {
    ::state.save(2);
  }

  SaveState3() : HotkeyInput("Save Quick State 3", "input.userInterface.states.saveQuickState3") {
    userInterfaceStates.attach(this);
  }
} saveState3;

}
