#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <vector>
#include <glm/glm.hpp>

#include "glad/glad.h"
#include "wrapping.hpp"

struct Texture {
  /* Needed in `Framebuffer` class */
  GLuint id;
  GLuint type;
  GLenum format;

  Texture();
  Texture(GLuint t, GLenum index=GL_TEXTURE0, Wrapping wrapping=Wrapping::REPEAT);
  GLenum get_index() const;
  void attach();
  int get_n_channels() const;

protected:
  GLenum m_index;

  /* whether texture is repeated, stretched or set to black beyond [0, 1] */
  Wrapping m_wrapping;

  void generate();
  void configure();
  void bind();
  void unbind();
  void set_format(int n_channels);
};

#endif // TEXTURE_HPP
