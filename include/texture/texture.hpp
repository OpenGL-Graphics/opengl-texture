#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <vector>
#include <glm/glm.hpp>

#include "glad/glad.h"
#include "image.hpp"
#include "wrapping.hpp"

template <class T>
struct Texture {
  GLuint id;
  int width;
  int height;
  GLuint type;

  Texture();
  Texture(const T& image, GLenum index=GL_TEXTURE0, Wrapping wrapping=Wrapping::REPEAT);
  Texture(GLuint id_tex, GLenum index=GL_TEXTURE0);
  void free() const;
  GLenum get_index() const;
  int get_width() const;
  int get_height() const;
  void attach();
  void set_image(const T& image=T());
  void set_subimage(const Image& subimage, const glm::uvec2& size, const glm::uvec2& offset);

private:
  // 2d or 3d (6-faced) image
  T m_image;

  GLenum m_index;

  /* whether texture is repeated, stretched or set to black beyond [0, 1] */
  Wrapping m_wrapping;

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
