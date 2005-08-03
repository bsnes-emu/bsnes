void editex_setbackcolor(HWND hwnd, COLORREF color) {
  SendMessage(hwnd, EM_SETBKGNDCOLOR, 0, color);
}

/*
  This command attempts to apply line color highlighting
  to a RichEdit 2.0 control. It tries to set the colors
  of blank lines, CPU message lines, APU message lines,
  and debug message lines to differing colors to aid in
  readability. It expects the input variable char *str
  to be in a special format where every line is exactly
  80 characters long. This is so that setting the
  background color of the line covers the entire width
  of the window.

  It auto-detects what type of message each line is by
  assuming formatting information returned by
  cpu->disassemble_op(n) and apu->disassemble_op(n).
*/
enum { EDITEX_NOALIGN = 0, EDITEX_ALIGNTOP, EDITEX_ALIGNBOTTOM };
void editex_settext(HWND hwnd, char *str, uint8 *linecol, uint8 align) {
char *t;
int sl, lines;
  SetWindowText(hwnd, str);
  if(!strcmp(str, ""))return;

  sl = strlen(str);
  lines = (sl + 1) / 80;

CHARFORMAT2 cf;
CHARRANGE   cr;
  memset(&cf, 0, sizeof(cf));
  cf.cbSize = sizeof(cf);
  cf.dwMask = CFM_COLOR | CFM_BACKCOLOR;
  cf.crTextColor = RGB(255, 255, 255);

int pos = 0;
  for(int i=0;i<lines;i++) {
    switch(linecol[i]) {
    case 0: //blank line
      cf.crBackColor = RGB(32, 32, 32);
      break;
    case 1: //CPU message
      cf.crBackColor = RGB(0, 0, 128);
      break;
    case 2: //APU message
      cf.crBackColor = RGB(144, 0, 0);
      break;
    case 3: //debug message
      cf.crBackColor = RGB(0, 64, 0);
      break;
    }

    cr.cpMin = pos;
    cr.cpMax = cr.cpMin + 80;
    pos += 81;
    SendMessage(hwnd, EM_EXSETSEL, 0, (LPARAM)&cr);
    SendMessage(hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
  }
  SendMessage(hwnd, EM_SETSEL, -1, -1);

//setting the cursor position has no effect on richedit controls,
//until focus is given to the window. even then, behavior is
//extremely unreliable. as a workaround to scroll the textbox to
//the bottom, we send the scrollbar a message, faking a user request
//to scroll to the bottom of the text window.
  switch(align) {
  case EDITEX_ALIGNTOP:
    SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);
    break;
  case EDITEX_ALIGNBOTTOM:
    SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);
    break;
  }
}
