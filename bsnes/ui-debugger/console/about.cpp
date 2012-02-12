#include <data/laevateinn.hpp>
AboutWindow *aboutWindow = nullptr;

AboutWindow::AboutWindow() {
  setTitle("About Laevateinn");
//setResizable(false);

  layout.setMargin(10);
  layout.setAlignment(0.5);
  canvas.setSize({288, 360});
  title.setFont("Sans, 16, Bold");
  title.setText("Laevateinn");
  version.setFont("Sans, 8, Bold");
  version.setText({"bsnes/debugger v", Version});
  website.setFont("Sans, 8, Bold");
  website.setText("http://byuu.org/");

  layout.append(canvas, {288, 360});
  layout.append(title, {0, 0});
  layout.append(version, {0, 0});
  layout.append(website, {0, 0});
  append(layout);

  image logo(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
  logo.loadPNG(laevateinnLogo, sizeof laevateinnLogo);
  logo.alphaBlend(backgroundColor().rgb());
  canvas.setImage(logo);
  canvas.update();

  setGeometry({800, 64, layout.minimumGeometry().width, layout.minimumGeometry().height});
}
