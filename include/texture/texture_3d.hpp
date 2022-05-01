#ifndef TEXTURE_3D_HPP
#define TEXTURE_3D_HPP

#include <vector>
#include <glm/glm.hpp>

#include "glad/glad.h"
#include "image.hpp"
#include "wrapping.hpp"
#include "texture.hpp"

struct Texture3D : Texture {
  // 3d (6-faced) images
  std::vector<Image> images;

  Texture3D(const std::string& path_image, GLenum index=GL_TEXTURE0, Wrapping wrapping=Wrapping::REPEAT);
  Texture3D(const std::vector<Image>& img, GLenum index=GL_TEXTURE0, Wrapping wrapping=Wrapping::REPEAT);
  void free() const;

private:
  void from_images();
};

#endif // TEXTURE_3D_HPP
