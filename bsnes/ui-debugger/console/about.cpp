#include <data/laevateinn.hpp>
AboutWindow *aboutWindow = nullptr;

AboutWindow::AboutWindow() {
  setTitle("About Laevateinn");
  setResizable(false);

  layout.setMargin(10);
  canvas.setSize({288, 360});
  title.setFont("Sans, 16, Bold");
  title.setText("Laevateinn");
  version.setFont("Sans, 8, Bold");
  version.setText({"bsnes/debugger v", Version});

  layout.append(canvas, {288, 360});
  layout.append(titleLayout, {~0, 0});
    titleLayout.append(titleL, {~0, 0});
    titleLayout.append(title, {0, 0});
    titleLayout.append(titleR, {~0, 0});
  layout.append(versionLayout, {~0, 0});
    versionLayout.append(versionL, {~0, 0});
    versionLayout.append(version, {0, 0});
    versionLayout.append(versionR, {~0, 0});
  append(layout);
}

void AboutWindow::show() {
  setVisible();
  setGeometry({800, 64, layout.minimumGeometry().width, layout.minimumGeometry().height});

  image logo(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
  logo.loadPNG(laevateinnLogo, sizeof laevateinnLogo);
  logo.alphaBlend(backgroundColor().rgb());
  canvas.setImage(logo);
  canvas.update();
}
