#include "texture.hpp"

/*
 * template specialization for 2d surface textures
 * Set texture image
 * Used to update texture image from loaded path in `imgui-example` project
 */
template <>
void Texture2D::set_image(const Image& image) {
  // 2d texture from given image (save width & height for HUD scaling)
  m_image = image;
  width = m_image.width;
  height = m_image.height;

  // copy image to gpu (image pointer could be freed after `glTexImage2D`)
  bind();
  glTexImage2D(GL_TEXTURE_2D, 0, m_image.format, width, height, 0, m_image.format, GL_UNSIGNED_BYTE, m_image.data);
  unbind();
}

template <>
Texture2D::Texture(const Image& image, GLenum index, Wrapping wrapping):
  m_type(GL_TEXTURE_2D),
  m_index(index),
  m_wrapping(wrapping)
{
  // default constructor needed to init class member TextRenderer::m_glyphs
  if (image.data != NULL) {
    generate();
    configure();
    set_image(image);
  }
}

/**
 * Used to construct a texture from its `id` passed to callback in <imgui-example> project
 * `index` needed in `Program::set_uniforms()`
 */
template <>
Texture2D::Texture(GLuint id_tex, GLenum index):
  m_type(GL_TEXTURE_2D),
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
void Texture2D::free() {
  m_image.free();
  glDeleteTextures(1, &id);
}

/*
 * template specialization for 3d cubic textures
 */
template <>
void Texture3D::from_images() {
  bind();

  // 6-sided texture cube using given images
  for (size_t i_texture = 0; i_texture < m_image.size(); i_texture++) {
    Image image(m_image[i_texture]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i_texture, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
  }

  unbind();

  // free images pointers
  for (const Image& image : m_image) {
    image.free();
  }
}

template <>
Texture3D::Texture(const std::vector<Image>& image, GLenum index, Wrapping wrapping):
  m_image(image),
  m_type(GL_TEXTURE_CUBE_MAP),
  m_index(index),
  m_wrapping(wrapping)
{
  generate();
  configure();
  from_images();
}

template <>
void Texture3D::free() {
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
  glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

  glTexParameteri(m_type, GL_TEXTURE_WRAP_S, wrapping_method);
  glTexParameteri(m_type, GL_TEXTURE_WRAP_T, wrapping_method);

  unbind();
}

template <class T>
void Texture<T>::bind() {
  glBindTexture(m_type, id);
}

template <class T>
void Texture<T>::unbind() {
  glBindTexture(m_type, 0);
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
