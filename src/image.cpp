#include <iostream>

#include "image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

/**
 * Load image
 * @param p Image path
 * @param desired_channels =1: load image in grayscale mode (heightmap), =0: no preference
 */
Image::Image(const std::string& p, int desired_channels):
  path(p)
{
 // load image using its path (opengl origin at lower-left corner of image)
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(path.c_str(), &width, &height, &n_channels, desired_channels);

  if (data == nullptr) {
    std::cout << "Image " << path << " doesn't exist" << std::endl;
  }

  set_format_from_n_channels();
}

/* Set image format from # of channels */
void Image::set_format_from_n_channels() {
  switch (n_channels) {
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
  }
}

/* Set # of channels from image format */
void Image::set_n_channels_from_format() {
  switch (format) {
    case GL_RED:
      n_channels = 1;
      break;
    case GL_RGB:
      n_channels = 3;
      break;
    case GL_RGBA:
      n_channels = 4;
      break;
  }
}

/**
 * Used to load glyph bitmap for a font into image (note that `path` is empty in this case, see `free()`) &
 * as default constructor (TextRenderer::m_glyphs contains textures that need to be init)
 * Also used to init processed image in <imgui-example>
 */
Image::Image(int w, int h, GLenum f, unsigned char* ptr, const std::string& p):
  width(w),
  height(h),
  format(f),
  data(ptr),
  path(p)
{
  set_n_channels_from_format();
}

void Image::free() const {
  // glyph bitmap don't have a path (avoid double free, as freed auto by freetype)
  if (!path.empty()) {
    stbi_image_free(data);
  }
}
