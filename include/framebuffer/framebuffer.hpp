#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "glad/glad.h"
#include "texture.hpp"

/**
 * Wrapper around OpenGL's framebuffer object (FBO)
 * https://learnopengl.com/Advanced-OpenGL/Framebuffers
 */
class Framebuffer {
public:
  Framebuffer();
  void attach_texture(const Texture2D& texture);
  void free();

private:
  GLuint m_id;

  void generate();
  void bind();
  void unbind();
};

#endif
