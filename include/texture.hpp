#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <vector>

#include "glad/glad.h"
#include "image.hpp"

template <class T>
struct Texture {
  GLuint id;
  int width;
  int height;

  Texture(const T& image=T(), GLenum index=GL_TEXTURE0);
  void free();
  GLenum get_index() const;
  int get_width() const;
  int get_height() const;
  void attach();
  void set_image(const T& image=T());

private:
  // 2d or 3d (6-faced) image
  T m_image;

  GLuint m_type;
  GLenum m_index;

  void generate();
  void configure();
  void bind();
  void unbind();
  void from_images();
};

// type of texture for a single image (2d) or a vector (3d)
using Texture2D = Texture<Image>;
using Texture3D = Texture<std::vector<Image>>;

#endif // TEXTURE_HPP
