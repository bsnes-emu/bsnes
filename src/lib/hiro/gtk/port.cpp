char* userpath(char *output) {
  struct passwd *userinfo = getpwuid(getuid());
  if(userinfo) { strcpy(output, userinfo->pw_dir); }
  return output;
}

int mkdir(const char *path) {
  return mkdir(path, 0755);
}

int main(int argc, char *argv[]) {
  gtk_init(&argc,  &argv);
  libhiro::hiro().init();
  int result = hiromain(argc, argv);
  libhiro::hiro().term();
  return result;
}
