class CheatEditorWindow : public Window {
public:
Label    Header;
Listview CheatList;
Checkbox Enabled;
Label    CodeLabel;
Editbox  Code;
Label    DescLabel;
Editbox  Desc;
Button   AddCode;
Button   EditCode;
Button   DeleteCode;
Button   ClearCode;
Checkbox GlobalEnable;

  bool Event(EventInfo &info);

  void CheatAdd();
  void CheatEdit();
  void CheatDelete();
  void CheatClear();
  void Refresh();
  void Clear();
  void LoadItem(int n);
  void UpdateItem(int n);
  void AddItem(const char *status, const char *desc, const char *code, const char *result);

  void Show();
  void Setup();
} wCheatEditor;
