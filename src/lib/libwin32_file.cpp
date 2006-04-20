/* FileOpen()
 *
 * Filter format:
 *   "Text documents;*.txt,*.inf|All files;*.*"
 *
 * Return values:
 *   true  - File was selected
 *   false - File was not selected
 */
bool FileOpen(Window *owner, const char *filter, const char *default_dir, char *output) {
string dir;
  strcpy(dir, default_dir ? default_dir : "");
  replace(dir, "/", "\\");

string f, type, part;
  strcpy(f, "");
  split(type, "|", filter);
  for(int i = 0; i < count(type); i++) {
    split(part, ";", type[i]);
    if(count(part) != 2)continue;

    strcat(f, part[0]);
    strcat(f, " (");
    strcat(f, part[1]);
    strcat(f, ")|");
    replace(part[1], ",", ";");
    strcat(f, part[1]);
    strcat(f, "|");
  }
char *pf = strptr(f);
  for(int i = strlen(pf) - 1; i >= 0; i--) {
    if(pf[i] == '|')pf[i] = '\0';
  }

OPENFILENAME ofn;
  strcpy(output, "");
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize     = sizeof(ofn);
  ofn.hwndOwner       = (owner) ? owner->hwnd : 0;
  ofn.lpstrFilter     = pf;
  ofn.lpstrInitialDir = strptr(dir);
  ofn.lpstrFile       = output;
  ofn.nMaxFile        = MAX_PATH;
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt     = "";

  return GetOpenFileName(&ofn);
}
