#include "framebuffer.hpp"

Framebuffer::Framebuffer() {
  generate();
}

void Framebuffer::generate() {
  glGenFramebuffers(1, &m_id);
}

/* Attach 2D texture as a color buffer of framebuffer */
void Framebuffer::attach_texture(const Texture2D& texture) {
  bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.type, texture.id, 0); 
  unbind();
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void Framebuffer::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::free() {
  glDeleteFramebuffers(1, &m_id);
}
