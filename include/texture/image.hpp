#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include "glad/glad.h"

/* Wrapper around image pointer returned by `stbi_load()` */
struct Image {
  int width;
  int height;
  GLenum format;
  int n_channels;
  unsigned char* data;
  std::string path;

  Image();
  Image(const std::string& p, bool flip=true);
  Image(int w, int h, int n, unsigned char* ptr, bool needs_free=true);
  void free();

  void save(const std::string& filename);
  std::vector<unsigned char> get_pixel_value(unsigned int i_pixel);

  unsigned char** to_2d_array() const;
  static Image from_2d_array(unsigned char** data_2d, int width, int height, int n_channels);

private:
  /* Avoids double-free for font bitmaps & for Texture3D with same image copy on 6 faces */
  bool m_needs_free;

  GLenum get_format_from_n_channels(int n);
  int get_n_channels_from_format(GLenum f);
};

#endif // IMAGE_HPP
