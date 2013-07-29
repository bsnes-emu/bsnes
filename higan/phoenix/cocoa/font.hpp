namespace phoenix {

struct pFont {
  static string serif(unsigned size, string style);
  static string sans(unsigned size, string style);
  static string monospace(unsigned size, string style);
  static Size size(string font, string text);

  static NSFont* cocoaFont(string description);
  static Size size(NSFont* font, string text);
};

}
