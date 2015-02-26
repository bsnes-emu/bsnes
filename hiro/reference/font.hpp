namespace phoenix {

struct pFont {
  static string serif(unsigned size, string style);
  static string sans(unsigned size, string style);
  static string monospace(unsigned size, string style);
  static Size size(string font, string text);
};

}
