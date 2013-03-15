namespace phoenix {

struct pFont {
  static string serif(unsigned size, string style);
  static string sans(unsigned size, string style);
  static string monospace(unsigned size, string style);
  static Size size(const string &font, const string &text);

  static NSFont* cocoaFont(const string &description);
  static Size size(NSFont *font, const string &text);
};

}
