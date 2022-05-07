#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "glad/glad.h"
#include "texture_2d.hpp"

/**
 * Wrapper around OpenGL's framebuffer object (FBO)
 * https://learnopengl.com/Advanced-OpenGL/Framebuffers
 */
struct Framebuffer {
  int width;
  int height;
  int n_channels;

  Framebuffer(const Texture2D& texture);
  bool is_complete();
  void free();
  void bind();
  void unbind();
  void clear(const glm::vec4& color);
  void get_pixel_value(int x, int y, unsigned char* data);

private:
  GLuint m_id;
  GLenum m_format;

  void generate();
  void attach_texture(const Texture2D& texture);
};

#endif
