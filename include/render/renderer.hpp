#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

#include "program.hpp"
#include "uniforms.hpp"
#include "vertexes/attribute.hpp"
#include "vertexes/vao.hpp"
#include "vertexes/vbo.hpp"
#include "math/transformation.hpp"

struct Renderer {
  /* accessed in derived class `TextRenderer` & in `Player` */
  VBO vbo;

  /* used to switch shaders in <imgui-paint> */
  Program program;

  Renderer(const Program& pgm, const Geometry& geometry, const std::vector<Attribute>& attributes, bool is_text=false);
  virtual void free() final;

  template <size_t N_INSTANCES = 1>
  void set_transform(const Transformation<N_INSTANCES>& transform);

  void draw(const Uniforms& u);
  void draw_plane(const Uniforms& u);
  void draw_lines(const Uniforms& u, unsigned int n_elements=0, size_t offset=0);
  void draw_with_outlines(const Uniforms& u);

private:
  VAO m_vao;
  Uniforms m_uniforms;

  /* # of instances of same geometry rendered at once: https://learnopengl.com/Advanced-OpenGL/Instancing */
  GLsizei m_n_instances;

  void _draw(const Uniforms& u, GLenum mode, unsigned int n_elements=0, size_t offset=0);
  void set_uniforms(const Uniforms& u);
};

#endif // RENDERER_HPP
