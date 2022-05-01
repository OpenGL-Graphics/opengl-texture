#include "texture.hpp"

/* Used by children constructors to init this class's members */
Texture::Texture(GLuint t, GLenum index, Wrapping wrapping):
  type(t),
  m_index(index),
  m_wrapping(wrapping)
{
}

void Texture::generate() {
  glGenTextures(1, &id);
}

void Texture::configure() {
  bind();

  // interpolation used when down/upscaling
  glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // wrapping method
  GLint wrapping_method;
  switch (m_wrapping) {
    case Wrapping::REPEAT:
      wrapping_method = GL_REPEAT;
      break;
    case Wrapping::STRETCH:
      wrapping_method = GL_CLAMP_TO_EDGE;
      break;
    case Wrapping::BLACK:
      wrapping_method = GL_CLAMP_TO_BORDER;
      break;
  }

  glTexParameteri(type, GL_TEXTURE_WRAP_S, wrapping_method);
  glTexParameteri(type, GL_TEXTURE_WRAP_T, wrapping_method);

  unbind();
}

void Texture::bind() {
  glBindTexture(type, id);
}

void Texture::unbind() {
  glBindTexture(type, 0);
}

/* Attach texture object id to texture unit m_index before `Renderer::draw()` */
void Texture::attach() {
  glActiveTexture(m_index);
  bind();
}

GLenum Texture::get_index() const {
  // used to pass texture index to shaders program
  return m_index - GL_TEXTURE0;
}
