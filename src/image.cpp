#include <iostream>

#include "image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

/**
 * Load image
 * @param p Image path
 * @param flip: images (origin at upper-left) need to be flipped horizontally in OpenGL 3D (origin at bottom)
 * but not in ImGui because of 2D projection matrix used in project <imgui-example>
 */
Image::Image(const std::string& p, bool flip):
  path(p)
{
  // opengl origin at lower-left corner of image
  if (flip) {
    stbi_set_flip_vertically_on_load(true);
  }

  // load image using its path
  int desired_channels = 0;
  data = stbi_load(path.c_str(), &width, &height, &n_channels, desired_channels);

  if (data == nullptr) {
    std::cout << "Image " << path << " doesn't exist" << std::endl;
  }

  set_format_from_n_channels();
}

/* Save image in jpeg format */
void Image::save(const std::string& filename) {
  stbi_write_jpg(filename.c_str(), width, height, n_channels, data, 90);
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

/**
 * Get pixel value at position i_pixel
 * @returns Vector containing `n_channels` components (monochrome, rgb, or rgba)
 */
std::vector<unsigned char> Image::get_pixel_value(unsigned int i_pixel) {
  switch (n_channels){
    case 4: // rgba
      return {
        data[n_channels*i_pixel],
        data[n_channels*i_pixel + 1],
        data[n_channels*i_pixel + 2],
        data[n_channels*i_pixel + 3],
      };
    case 3: // rgb
      return {
        data[n_channels*i_pixel],
        data[n_channels*i_pixel + 1],
        data[n_channels*i_pixel + 2],
      };
  }

  // monochrome
  return {
    data[i_pixel],
  };
}
