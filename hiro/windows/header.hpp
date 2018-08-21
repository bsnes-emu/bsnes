#include <nall/windows/guard.hpp>
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <commdlg.h>
#include <uxtheme.h>
#include <io.h>
#include <shlobj.h>
#include <nall/windows/guard.hpp>
#include <nall/windows/registry.hpp>
#include <nall/windows/utf8.hpp>

//MinGW/32-bit has painfully outdated platform headers ...

#if !defined(Button_SetImageList)
  typedef struct {
    HIMAGELIST himl;
    RECT margin;
    UINT uAlign;
  } BUTTON_IMAGELIST, *PBUTTON_IMAGELIST;

  #define BUTTON_IMAGELIST_ALIGN_LEFT 0
  #define BUTTON_IMAGELIST_ALIGN_RIGHT 1
  #define BUTTON_IMAGELIST_ALIGN_TOP 2
  #define BUTTON_IMAGELIST_ALIGN_BOTTOM 3
  #define BUTTON_IMAGELIST_ALIGN_CENTER 4

  #define BCM_FIRST 0x1600
  #define BCM_SETIMAGELIST (BCM_FIRST+2)
  #define Button_SetImageList(hwnd, pbuttonImagelist) (WINBOOL)SNDMSG((hwnd),BCM_SETIMAGELIST,0,(LPARAM)(pbuttonImagelist))
#endif

#if !defined(BP_PUSHBUTTON)
  #define BP_PUSHBUTTON 1
#endif

#if !defined(PBS_NORMAL)
  #define PBS_NORMAL 1
#endif

#if !defined(PBS_HOT)
  #define PBS_HOT 2
#endif

#if !defined(PBS_PRESSED)
  #define PBS_PRESSED 3
#endif

#if !defined(PBS_DISABLED)
  #define PBS_DISABLED 4
#endif

#if !defined(PBS_DEFAULTED)
  #define PBS_DEFAULTED 5
#endif

#if !defined(BP_CHECKBOX)
  #define BP_CHECKBOX 3
#endif

#if !defined(CBS_UNCHECKEDNORMAL)
  #define CBS_UNCHECKEDNORMAL 1
#endif

#if !defined(CBS_CHECKEDNORMAL)
  #define CBS_CHECKEDNORMAL 5
#endif

#if !defined(LVCFMT_FIXED_WIDTH)
  #define LVCFMT_FIXED_WIDTH 0x0100
#endif

#if !defined(TBS_TRANSPARENTBKGND)
  #define TBS_TRANSPARENTBKGND 0x1000
#endif

#if !defined(TTP_STANDARD)
  #define TTP_STANDARD 1
#endif

#if !defined(TTSS_NORMAL)
  #define TTSS_NORMAL 1
#endif
