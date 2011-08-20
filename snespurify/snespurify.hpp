#include <nall/directory.hpp>
#include <nall/file.hpp>
#include <nall/ips.hpp>
#include <nall/ups.hpp>
#include <nall/vector.hpp>
#include <nall/zip.hpp>
#include <nall/bps/linear.hpp>
#include <phoenix/phoenix.hpp>
using namespace nall;
using namespace phoenix;

struct MainWindow : public Window {
  Font font;
  VerticalLayout layout;
  HorizontalLayout navigationLayout;
  Label navigationLabel;
  LineEdit navigationPath;
  Button navigationScan;
  Button navigationBrowse;
  ListView listView;
  HorizontalLayout controlLayout;
  Button controlSelectAll;
  Button controlClearAll;
  CheckBox controlDeleteFiles;
  Widget spacer;
  Button controlCorrect;

  void create();
  void onScan();
  void onScanFolder(const string &path);
  void analyzeFile(const string &filename);

  bool isSnesCartImageName(const string &filename);
  bool isSnesBsImageName(const string &filename);
  bool isSnesStImageName(const string &filename);
  bool isSnesImageName(const string &filename);
  bool isGameBoyClassicImageName(const string &filename);
  bool isGameBoyColorImageName(const string &filename);
  bool isGameBoyImageName(const string &filename);
  bool isImageName(const string &filename);
  bool isGoodImageName(const string &filename);
  bool isBadImageName(const string &filename);
  string correctExtension(const string &filename);

  struct File {
    string name;
    string problem;
  };
  linear_vector<File> fileList;
  lstring errors;

  void onCorrect();
  void problemDecompressZip(File &file);
  void problemCorrectExtension(File &file);
  void problemCreatePatchFromUPS(File &file);
  void problemCreatePatchFromIPS(File &file);
  void problemRemoveCopierHeader(File &file);
};
