#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>

#include "render/renderer.hpp"
#include "uniforms.hpp"

/**
 * @param is_text Text has a dynamically-draw vbo
 * TODO: Program should be passed to Renderer::draw()
 */
Renderer::Renderer(const Program& pgm, const Geometry& geometry, const std::vector<Attribute>& attributes, bool is_text):
  m_vao(),
  vbo(geometry, is_text),
  program(pgm),
  m_n_instances(1)
{
  // create vertex attributes linking bound VAO and VBO (& EBO with it)
  m_vao.bind();
  vbo.bind();

  for (const Attribute& attribute : attributes) {
    m_vao.set_attribute(attribute);
  }

  m_vao.unbind();
  vbo.unbind();
}

/**
 * Sets transformation matrix (translation, rotation, scaling) in uniforms
 * glm::mat4 used as certain objects in scene require a scaling (besides translation)
 * Update 20-01-23: # of instances to draw determined from # of model matrices (i.e. positions)
 */
template <size_t N_INSTANCES>
void Renderer::set_transform(const Transformation<N_INSTANCES>& transform) {
  /*
  for (size_t i_instance = 0; i_instance < N_INSTANCES; ++i_instance) {
    std::stringstream stream;
    stream << "models[" << i_instance  << "]";
    m_uniforms[stream.str()] = transform.models[i_instance];
  }
  */
  set_uniform_arr("models", transform.models);

  m_uniforms["view"] = transform.view;
  m_uniforms["projection"] = transform.projection;
  m_n_instances = N_INSTANCES;
}

/* Different functions for every type of primitive (triangles, strips, lines) */
void Renderer::draw(const Uniforms& u) {
  _draw(u, GL_TRIANGLES, vbo.n_elements);
}

void Renderer::draw_plane(const Uniforms& u) {
  _draw(u, GL_TRIANGLE_STRIP, vbo.n_elements);
}

void Renderer::draw_lines(const Uniforms& u, unsigned int n_elements, size_t offset) {
  unsigned int count = (n_elements == 0) ? vbo.n_elements : n_elements;
  _draw(u, GL_LINES, count, offset);
}

/* TODO: move this method & next one to separate class */
template <size_t N_INSTANCES, typename T>
void Renderer::set_uniform_arr(const std::string& name, const std::array<T, N_INSTANCES>& u) {
  // case of array of structs fields (see lights uniform in phong shader)
  size_t i_dot = name.find(".");
  bool is_struct_field = (i_dot != std::string::npos);

  std::string field_name, struct_name;
  if (is_struct_field) {
    struct_name = name.substr(0, i_dot);
    field_name = name.substr(i_dot + 1);
  }

  for (size_t i_instance = 0; i_instance < N_INSTANCES; ++i_instance) {
    std::stringstream stream;
    if (!is_struct_field)
      stream << name << "[" << i_instance  << "]";
    else
      stream << struct_name << "[" << i_instance  << "]." << field_name;

    m_uniforms[stream.str()] = u[i_instance];
  }
}

/* Append to uniforms field member */
void Renderer::set_uniforms(const Uniforms& u) {
  for (const auto& item : u) {
    KeyUniform key_uniform(item.first);
    ValueUniform value_uniform(item.second);
    m_uniforms[key_uniform] = value_uniform;
  }
}

/**
 * Draw vertexes given to `vbo`
 * @param Uniforms Unordered map (key, values) of vars to pass to shader
 *        Passed as const ref. cos non-const ref. not allowed to bind to temps (initializer_list)
 * @param mode GL_TRIANGLES for most meshes, GL_TRIANGLE_STRIP for grids (i.e. terrain & plane)
 *             GL_LINES to draw a line between each pair of successive vertexes
 * @param n_elements # of elements (i.e. indices) to draw (used only by `geometry/gizmo.cpp`)
 * @param offset Indice to start rendering from in EBO (used only by `geometry/gizmo.cpp`)
 */
void Renderer::_draw(const Uniforms& u, GLenum mode, unsigned int n_elements, size_t offset) {
  set_uniforms(u);

  // wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // pass shaders uniforms & draw attributes in bound VAO (using EBO vertexes indices)
  // offset given to `glDrawElements()` in bytes
  m_vao.bind();
  program.use();

  program.set_uniforms(m_uniforms);
  glDrawElementsInstanced(mode, n_elements, GL_UNSIGNED_INT, (GLvoid *) (offset * sizeof(GLuint)), m_n_instances);

  m_vao.unbind();
  program.unuse();
}

/**
 * Draw vertexes with outlines using stencil buffer & multipass drawing
 * @param Uniforms Unordered map (key, values) of vars to pass to shader
 */
void Renderer::draw_with_outlines(const Uniforms& u) {
  // clear stencil mask (so drawing doesn't have to happen at very beginning of main loop)
  glClear(GL_STENCIL_BUFFER_BIT);

  // 1st render pass: always pass the stencil test & set ref=1 in stencil buffer for drawn fragments (pixels)
  glStencilFunc(GL_ALWAYS, 1, 0xff);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  draw(u);

  // get transformation matrixes from uniforms
  glm::mat4 model = std::get<glm::mat4>(m_uniforms["models[0]"]);
  glm::mat4 view = std::get<glm::mat4>(m_uniforms["view"]);
  glm::mat4 projection = std::get<glm::mat4>(m_uniforms["projection"]);

  // 2nd render pass: only fragments that weren't assigned 1s in previous step are rendered (pass the test)
  // white outlines & scaling
  glStencilFunc(GL_NOTEQUAL, 1, 0xff);
  set_transform({
    { glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f)) },
    view,
    projection
  });
  Uniforms uniforms = u;
  uniforms["colors[0]"] = glm::vec3(1.0f, 1.0f, 1.0f);
  draw(uniforms);

  // reset stencil test to always pass (for further rendering in same frame)
  glStencilFunc(GL_ALWAYS, 1, 0xff);
}

/* Free used vertex buffers (VAO & VBO lifecycles managed by renderer) & shader program */
void Renderer::free() {
  m_vao.free();
  vbo.free();
}

// template instantiation to avoid linking error
// TODO: use boost preprocessing to loop over instantiations: https://stackoverflow.com/a/71885137/2228912
template void Renderer::set_transform<1>(const Transformation<1>& transform);
template void Renderer::set_transform<2>(const Transformation<2>& transform);
template void Renderer::set_transform<3>(const Transformation<3>& transform);

template void Renderer::set_uniform_arr(const std::string& name, const std::array<glm::vec3, 2>& u);
template void Renderer::set_uniform_arr(const std::string& name, const std::array<glm::vec3, 3>& u);
