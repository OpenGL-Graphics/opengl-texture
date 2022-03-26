#include "texture.hpp"

/**
 * template specialization for 2d surface textures
 * Retrieve image data from opengl texture (gpu -> cpu) & assign it to internal image
 * Called before saving image the user painted on with nanovg in <imgui-paint>
 */
template <>
void Texture2D::get_image() {
  unsigned char* data = new unsigned char[width * height * image.n_channels];
  glGetTexImage(type, 0, image.format, GL_UNSIGNED_BYTE, data);

  // free previous image & assign retrieved one
  image.free();
  image.data = data;
}

/*
 * template specialization for 2d surface textures
 * Update only subset of image texture
 * Used in `imgui-example` project so Brush tool is more fluid (no discontinuities between circles)
 */
template <>
void Texture2D::set_subimage(const Image& subimage, const glm::uvec2& size, const glm::uvec2& offset) {
  // copy image subset to gpu (subimage pointer freed from calling code)
  bind();
  glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, size.x, size.y, subimage.format, GL_UNSIGNED_BYTE, subimage.data);
  unbind();
}

/*
 * template specialization for 2d surface textures
 * Set texture image
 * Used to update texture image from loaded path in `imgui-example` project
 */
template <>
void Texture2D::set_image(const Image& img) {
  // free pixel data from previous image
  image.free();

  // 2d texture from given image (save width & height for HUD scaling)
  image = img;
  width = image.width;
  height = image.height;

  // copy image to gpu (image pointer could be freed after `glTexImage2D`)
  bind();
  glTexImage2D(GL_TEXTURE_2D, 0, image.format, width, height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
  unbind();
}

/* Default constructor used by canvas in `imgui-paint` to create the texture that fbo draw on */
template <>
Texture2D::Texture() {}

template <>
Texture2D::Texture(const Image& img, GLenum index, Wrapping wrapping):
  type(GL_TEXTURE_2D),
  m_index(index),
  m_wrapping(wrapping)
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
template <>
Texture2D::Texture(GLuint id_tex, GLenum index):
  type(GL_TEXTURE_2D),
  m_index(index),
  m_wrapping(Wrapping::BLACK)
{
  // empty as texture has already been defined & configured before callback is called
}

template <>
int Texture2D::get_width() const {
  // used to scale HUD surface
  return width;
}

template <>
int Texture2D::get_height() const {
  // used to scale HUD surface
  return height;
}

/**
 * Delete texture & free associated image
 * Image not freed just after `glTexImage2D()` to process it in <imgui-example>
 */
template <>
void Texture2D::free() const {
  image.free();
  glDeleteTextures(1, &id);
}

/*
 * template specialization for 3d cubic textures
 */
template <>
void Texture3D::from_images() {
  bind();

  // 6-sided texture cube using given images
  for (size_t i_texture = 0; i_texture < image.size(); i_texture++) {
    Image img(image[i_texture]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i_texture, 0, img.format, img.width, img.height, 0, img.format, GL_UNSIGNED_BYTE, img.data);
  }

  unbind();

  // free images pointers
  for (const Image& img : image) {
    img.free();
  }
}

template <>
Texture3D::Texture(const std::vector<Image>& img, GLenum index, Wrapping wrapping):
  image(img),
  type(GL_TEXTURE_CUBE_MAP),
  m_index(index),
  m_wrapping(wrapping)
{
  generate();
  configure();
  from_images();
}

template <>
void Texture3D::free() const {
  glDeleteTextures(1, &id);
}

/*
 * generic methods
 */
template <class T>
void Texture<T>::generate() {
  glGenTextures(1, &id);
}

template <class T>
void Texture<T>::configure() {
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

template <class T>
void Texture<T>::bind() {
  glBindTexture(type, id);
}

template <class T>
void Texture<T>::unbind() {
  glBindTexture(type, 0);
}

/* Attach texture object id to texture unit m_index before `Renderer::draw()` */
template <class T>
void Texture<T>::attach() {
  glActiveTexture(m_index);
  bind();
}

template <class T>
GLenum Texture<T>::get_index() const {
  // used to pass texture index to shaders program
  return m_index - GL_TEXTURE0;
}

// template instantiation to generate class from it (fixes link error)
template class Texture<Image>;
template class Texture<std::vector<Image>>;
