/* hiro components
 *
 * By commenting out lines below, individual components of hiro can be disabled.)
 * This can be useful to avoid dependencies (eg GTK+ relies on GtkSourceView for SourceEdit.)
 * It's also very useful for porting hiro to new targets; or performing major core changes.
 *
 * Note that the core classes (Application, Window, Sizable, etc) have circular dependencies.
 * Disabling only certain core pieces will result in compilation errors.
 * As such, this file is really only meant for disabling individual widgets or menu items.
 */

#define Hiro_Color
#define Hiro_Gradient
#define Hiro_Alignment
#define Hiro_TextCursor
#define Hiro_Position
#define Hiro_Size
#define Hiro_Geometry
#define Hiro_Font
#define Hiro_MouseCursor

#define Hiro_Application
#define Hiro_Desktop
#define Hiro_Monitor
#define Hiro_Keyboard
#define Hiro_Mouse
#define Hiro_BrowserWindow
#define Hiro_MessageWindow

#define Hiro_Attribute

#define Hiro_Object
#define Hiro_Group

#define Hiro_Hotkey
#define Hiro_Timer

#define Hiro_Window
#define Hiro_StatusBar
#define Hiro_MenuBar
#define Hiro_PopupMenu

#define Hiro_Action
#define Hiro_Menu
#define Hiro_MenuSeparator
#define Hiro_MenuItem
#define Hiro_MenuCheckItem
#define Hiro_MenuRadioItem

#define Hiro_Sizable
#define Hiro_Widget
#define Hiro_Button
#define Hiro_Canvas
#define Hiro_CheckButton
#define Hiro_CheckLabel
#define Hiro_ComboButton
#define Hiro_ComboEdit
#define Hiro_Console
#define Hiro_Frame
#define Hiro_HexEdit
#define Hiro_HorizontalScrollBar
#define Hiro_HorizontalSlider
#define Hiro_IconView
#define Hiro_Label
#define Hiro_LineEdit
#define Hiro_ProgressBar
#define Hiro_RadioButton
#define Hiro_RadioLabel
#define Hiro_SourceEdit
#define Hiro_TabFrame
#define Hiro_TableView
#define Hiro_TextEdit
#define Hiro_TreeView
#define Hiro_VerticalScrollBar
#define Hiro_VerticalSlider
#define Hiro_Viewport

//platform-specific exclusions

#if defined(HIRO_WINDOWS) || defined(HIRO_COCOA) || defined(HIRO_QT) || defined(HIRO_NO_SOURCEVIEW)
  #undef Hiro_ComboEdit
  #undef Hiro_Console
  #undef Hiro_IconView
  #undef Hiro_SourceEdit
  #undef Hiro_TreeView
#endif

#if defined(HIRO_COCOA)
  #undef Hiro_HexEdit
#endif

//extensions

#define Hiro_FixedLayout
#define Hiro_HorizontalLayout
#define Hiro_VerticalLayout
#define Hiro_TableLayout

#if defined(Hiro_Timer) && defined(Hiro_Canvas)
  #define Hiro_HorizontalResizeGrip
  #define Hiro_VerticalResizeGrip
#endif

#if defined(Hiro_TableView)
  #define Hiro_ListView
#endif

#if defined(Hiro_Button) && defined(Hiro_Canvas) && defined(Hiro_Label)
  #define Hiro_MessageDialog
  #define Hiro_NameDialog
#endif

#if defined(Hiro_Button) && defined(Hiro_ComboButton) && defined(Hiro_LineEdit) && defined(Hiro_ListView) && defined(Hiro_MessageDialog) && defined(Hiro_NameDialog)
  #define Hiro_BrowserDialog
#endif

#if defined(Hiro_Canvas) && defined(Hiro_Label)
  #define Hiro_AboutDialog
#endif
