#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>

#include "glad/glad.h"

/* Wrapper around image pointer returned by `stbi_load()` */
struct Image {
  int width;
  int height;
  GLenum format;
  int n_channels;
  unsigned char* data;
  std::string path;

  Image(const std::string& p, bool flip=true);
  Image(int w=0, int h=0, GLenum f=0, unsigned char* ptr=NULL, const std::string& p="");
  void free() const;

private:
  void set_format_from_n_channels();
  void set_n_channels_from_format();
};

#endif // IMAGE_HPP
