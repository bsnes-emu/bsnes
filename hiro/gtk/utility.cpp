namespace hiro {

#if defined(Hiro_Color)
static auto CreateColor(const Color& color) -> GdkColor {
  GdkColor gdkColor;
  gdkColor.pixel = (color.red() << 16) | (color.green() << 8) | (color.blue() << 0);
  gdkColor.red = (color.red() << 8) | (color.red() << 0);
  gdkColor.green = (color.green() << 8) | (color.green() << 0);
  gdkColor.blue = (color.blue() << 8) | (color.blue() << 0);
  return gdkColor;
}
#endif

static auto CreatePixbuf(const nall::image& image, bool scale = false) -> GdkPixbuf* {
  if(!image) return nullptr;

  nall::image gdkImage = image;
  gdkImage.transform(0, 32, 255u << 24, 255u << 0, 255u << 8, 255u << 16);
  if(scale) gdkImage.scale(15, 15);

  GdkPixbuf* pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, gdkImage.width, gdkImage.height);
  memcpy(gdk_pixbuf_get_pixels(pixbuf), gdkImage.data, gdkImage.width * gdkImage.height * 4);

  return pixbuf;
}

static auto CreateImage(const nall::image& image, bool scale = false) -> GtkImage* {
  GdkPixbuf* pixbuf = CreatePixbuf(image, scale);
  GtkImage* gtkImage = (GtkImage*)gtk_image_new_from_pixbuf(pixbuf);
  g_object_unref(pixbuf);
  return gtkImage;
}

static auto DropPaths(GtkSelectionData* data) -> lstring {
  gchar** uris = gtk_selection_data_get_uris(data);
  if(uris == nullptr) return {};

  lstring paths;
  for(unsigned n = 0; uris[n] != nullptr; n++) {
    gchar* pathname = g_filename_from_uri(uris[n], nullptr, nullptr);
    if(pathname == nullptr) continue;

    string path = pathname;
    g_free(pathname);
    if(directory::exists(path) && !path.endsWith("/")) path.append("/");
    paths.append(path);
  }

  g_strfreev(uris);
  return paths;
}

}
