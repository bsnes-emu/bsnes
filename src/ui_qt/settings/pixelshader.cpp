#include "pixelshader.moc"
PixelShaderWindow *pixelShaderWindow;

PixelShaderWindow::PixelShaderWindow() {
  layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(Style::WidgetSpacing);
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);

  gridLayout = new QGridLayout;
  gridLayout->setVerticalSpacing(0);
  layout->addLayout(gridLayout);

  fragmentLabel = new QLabel("Fragment shader:");
  gridLayout->addWidget(fragmentLabel, 0, 0, 1, 3);

  fragmentPath = new QLineEdit;
  gridLayout->addWidget(fragmentPath, 1, 0);

  fragmentSelect = new QPushButton("Select ...");
  gridLayout->addWidget(fragmentSelect, 1, 1);

  fragmentDefault = new QPushButton("Default");
  gridLayout->addWidget(fragmentDefault, 1, 2);

  vertexLabel = new QLabel("Vertex shader:");
  gridLayout->addWidget(vertexLabel, 2, 0, 1, 3);

  vertexPath = new QLineEdit;
  gridLayout->addWidget(vertexPath, 3, 0);

  vertexSelect = new QPushButton("Select ...");
  gridLayout->addWidget(vertexSelect, 3, 1);

  vertexDefault = new QPushButton("Default");
  gridLayout->addWidget(vertexDefault, 3, 2);

  synchronize();

  connect(fragmentSelect, SIGNAL(released()), this, SLOT(selectFragmentShader()));
  connect(vertexSelect, SIGNAL(released()), this, SLOT(selectVertexShader()));
  connect(fragmentDefault, SIGNAL(released()), this, SLOT(defaultFragmentShader()));
  connect(vertexDefault, SIGNAL(released()), this, SLOT(defaultVertexShader()));
}

void PixelShaderWindow::synchronize() {
  fragmentPath->setText(config().path.fragmentShader);
  vertexPath->setText(config().path.vertexShader);
}

void PixelShaderWindow::selectFragmentShader() {
  fileBrowser->onChange = (void*)0;
  fileBrowser->onActivate = bind(&PixelShaderWindow::assignFragmentShader, this);
  fileBrowser->onAccept = bind(&PixelShaderWindow::assignFragmentShader, this);
  fileBrowser->setWindowTitle("Select Fragment Shader");
  fileBrowser->setPath(config().path.current.shader);
  fileBrowser->setNameFilters("All files (*)");
  fileBrowser->showLoad();
}

void PixelShaderWindow::selectVertexShader() {
  fileBrowser->onChange = (void*)0;
  fileBrowser->onActivate = bind(&PixelShaderWindow::assignVertexShader, this);
  fileBrowser->onAccept = bind(&PixelShaderWindow::assignVertexShader, this);
  fileBrowser->setWindowTitle("Select Vertex Shader");
  fileBrowser->setPath(config().path.current.shader);
  fileBrowser->setNameFilters("All files (*)");
  fileBrowser->showLoad();
}

void PixelShaderWindow::defaultFragmentShader() { assignFragmentShader(""); }
void PixelShaderWindow::defaultVertexShader() { assignVertexShader(""); }

void PixelShaderWindow::assignFragmentShader(const string &filename) {
  if(filename == "" || QDir(filename).exists() == false) {
    config().path.fragmentShader = filename;
    if(filename != "") config().path.current.shader = dir(filename);
    synchronize();
    utility.updatePixelShader();
  }
}

void PixelShaderWindow::assignVertexShader(const string &filename) {
  if(filename == "" || QDir(filename).exists() == false) {
    config().path.vertexShader = filename;
    if(filename != "") config().path.current.shader = dir(filename);
    synchronize();
    utility.updatePixelShader();
  }
}
