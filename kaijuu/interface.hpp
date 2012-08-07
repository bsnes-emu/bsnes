#include <nall/platform.hpp>
#include <nall/invoke.hpp>
#include <nall/string.hpp>
#include <nall/windows/registry.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include "guid.hpp"
#include "settings.hpp"

struct Application : Window {
  VerticalLayout layout;
    HorizontalLayout installLayout;
      Label statusLabel;
      Button uninstallButton;
      Button installButton;
    HorizontalLayout settingLayout;
      ListView settingList;
      VerticalLayout controlLayout;
        Button appendButton;
        Button modifyButton;
        Button moveUpButton;
        Button moveDownButton;
        Button removeButton;
        Button resetButton;
        Widget spacer;
        Button helpButton;
        Canvas canvas;

  Application(const string &pathname);
  void synchronize();
  void refresh();
  void install();
  void uninstall();
  void appendAction();
  void modifyAction();
  void moveUpAction();
  void moveDownAction();
  void removeAction();
  void resetAction();

  string pathname;
};

struct RuleEditor : Window {
  VerticalLayout layout;
    HorizontalLayout nameLayout;
      Label nameLabel;
      LineEdit nameValue;
    HorizontalLayout patternLayout;
      Label patternLabel;
      LineEdit patternValue;
    HorizontalLayout commandLayout;
      Label commandLabel;
      LineEdit commandValue;
      Button commandSelect;
    HorizontalLayout controlLayout;
      CheckBox defaultAction;
      CheckBox filesAction;
      CheckBox foldersAction;
      Widget spacer;
      Button assignButton;

  bool modal;
  signed index;

  RuleEditor();
  void synchronize();
  void show(signed rule = -1);
};
