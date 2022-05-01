#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include <vector>
#include <glm/glm.hpp>

#include "glad/glad.h"
#include "image.hpp"
#include "wrapping.hpp"
#include "texture.hpp"

struct Texture2D : Texture {
  /* Needed in <imgui-paint> project */
  int width;
  int height;
  Image image;

  /**
   * Inherit parent constructor:
   * Needed by `Mesh` as its textures not init. till `Model` is constructed
   */
  using Texture::Texture;

  Texture2D(const Image& img, GLenum index=GL_TEXTURE0, Wrapping wrapping=Wrapping::REPEAT);
  Texture2D(GLuint id_tex, GLenum index=GL_TEXTURE0);
  void free();

  void set_image(const Image& img=Image());
  void set_subimage(const Image& subimage, const glm::uvec2& size, const glm::uvec2& offset);
  void get_image();
};

#endif // TEXTURE_2D_HPP
