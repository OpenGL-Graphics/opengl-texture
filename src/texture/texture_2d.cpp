#include "texture_2d.hpp"

/* Needed by `Mesh` as its textures not init. till `Model` in constructed */
Texture2D::Texture2D():
  Texture(GL_TEXTURE_2D)
{}

Texture2D::Texture2D(const Image& img, GLenum index, Wrapping wrapping):
  Texture(GL_TEXTURE_2D, index, wrapping)
{
  // default constructor needed because lvalue in assignment `map[key] = value` (source: models/models.cpp) evals to a reference
  // https://stackoverflow.com/a/29826440/2228912
  generate();
  configure();
  set_image(img);
}

/**
 * Used to construct a texture from its `id` passed to callback in <imgui-example> project
 * `index` needed in `Program::set_uniforms()`
 */
Texture2D::Texture2D(GLuint id_tex, GLenum index):
  Texture(GL_TEXTURE_2D, index, Wrapping::BLACK)
{
  // empty as texture has already been defined & configured before callback is called
}

/**
 * Retrieve image data from opengl texture (gpu -> cpu) & assign it to internal image
 * Called before saving image the user painted on with nanovg in <imgui-paint>
 */
void Texture2D::get_image() {
  unsigned char* data = new unsigned char[width * height * image.n_channels];
  glGetTexImage(type, 0, image.format, GL_UNSIGNED_BYTE, data);

  // free previous image & assign retrieved one
  image.free();
  image.data = data;
}

/*
 * Update only subset of image texture
 * Used in `imgui-example` project so Brush tool is more fluid (no discontinuities between circles)
 */
void Texture2D::set_subimage(const Image& subimage, const glm::uvec2& size, const glm::uvec2& offset) {
  // copy image subset to gpu (subimage pointer freed from calling code)
  bind();
  glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, size.x, size.y, subimage.format, GL_UNSIGNED_BYTE, subimage.data);
  unbind();
}

/*
 * Set texture image
 * Used to update texture image from loaded path in `imgui-example` project
 */
void Texture2D::set_image(const Image& img) {
  // 2d texture from given image (save width & height for HUD scaling)
  image = img;
  width = image.width;
  height = image.height;

  // copy image to gpu (image pointer could be freed after `glTexImage2D`)
  bind();
  glTexImage2D(GL_TEXTURE_2D, 0, image.format, width, height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
  unbind();

  // free image pointer
  image.free();
}

/**
 * Delete texture
 */
void Texture2D::free() {
  glDeleteTextures(1, &id);
}
