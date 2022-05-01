#include "texture_3d.hpp"

void Texture3D::from_images() {
  bind();

  // 6-sided texture cube using given images
  for (size_t i_texture = 0; i_texture < images.size(); i_texture++) {
    Image image = images[i_texture];
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i_texture, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
  }

  unbind();

  // free images pointers
  for (const Image& image : images) {
    image.free();
  }
}

/* Used to init all faces textures to same image */
Texture3D::Texture3D(const std::string& path_image, GLenum index, Wrapping wrapping):
  Texture3D(std::vector<Image>(6, Image(path_image)), index, wrapping)
{
}

Texture3D::Texture3D(const std::vector<Image>& imgs, GLenum index, Wrapping wrapping):
  images(imgs),
  Texture(GL_TEXTURE_CUBE_MAP, index, wrapping)
{
  generate();
  configure();
  from_images();
}

void Texture3D::free() const {
  glDeleteTextures(1, &id);
}
