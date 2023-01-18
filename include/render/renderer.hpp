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
  void set_transform(const Transformation& t);

  void draw(const Uniforms& u, GLsizei n_instances=1);
  void draw_plane(const Uniforms& u);
  void draw_lines(const Uniforms& u, unsigned int n_elements=0, size_t offset=0);
  void draw_with_outlines(const Uniforms& u);

private:
  VAO m_vao;
  Transformation m_transformation;

  void _draw(const Uniforms& u, GLenum mode, unsigned int n_elements=0, GLsizei n_instances=1, size_t offset=0);
};

#endif // RENDERER_HPP
