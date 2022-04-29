#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "program.hpp"
#include "shader.hpp"
#include "file.hpp"

/* Used to construct a program from its `id` passed to callback in <imgui-example> project */
Program::Program(GLuint id_pgm):
  id(id_pgm)
{
}

Program::Program(const std::string& path_vertex, const std::string& path_fragment) {
  // read shader source codes into strings (newer GLSL version not supported)
  std::string source_vertex = File::get_content(path_vertex);
  std::string source_fragment = File::get_content(path_fragment);

  // create vertex & fragment shaders
  Shader shader_vertex(source_vertex, GL_VERTEX_SHADER);
  Shader shader_fragment(source_fragment, GL_FRAGMENT_SHADER);
  GLuint vertex = shader_vertex.id;
  GLuint fragment = shader_fragment.id;
  if (vertex == 0 || fragment == 0) {
    id = 0;
    return;
  }

  // attach shaders to program & link it
  id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);
  glValidateProgram(id);

  // error handling of program linking
  GLint result_link;
  glGetProgramiv(id, GL_LINK_STATUS, &result_link);
  if (result_link == GL_FALSE) {
    GLint length_link;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length_link);
    std::vector<GLchar> message(length_link);
    glGetProgramInfoLog(id, length_link, NULL, message.data());
    std::cout << "program: " << message.data() << "\n";

    glDeleteProgram(id);
    id = 0;
  }

  // flag attached shaders objects for deletion
  shader_vertex.free();
  shader_fragment.free();
}

void Program::set_mat4(const std::string& name, const glm::mat4& mat) {
  // set value of uniform variable using its name
  GLuint uniform = glGetUniformLocation(id, name.c_str());
  if (uniform != -1)
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(mat));
}

void Program::set_vec3(const std::string& name, const glm::vec3& vect) {
  GLuint uniform = glGetUniformLocation(id, name.c_str());
  if (uniform != -1)
    glUniform3fv(uniform, 1, glm::value_ptr(vect));
}

void Program::set_bool(const std::string& name, bool value) {
  set_int(name, value);
}

void Program::set_int(const std::string& name, int value) {
  GLuint uniform = glGetUniformLocation(id, name.c_str());
  if (uniform != -1)
    glUniform1i(uniform, value);
}

void Program::set_float(const std::string& name, float value) {
  GLuint uniform = glGetUniformLocation(id, name.c_str());
  if (uniform != -1)
    glUniform1f(uniform, value);
}

void Program::use() {
  glUseProgram(id);
}

void Program::unuse() {
  glUseProgram(0);
}

void Program::free() {
  glDeleteProgram(id);
}

bool Program::has_failed() {
  return (id == 0) ? true : false;
}

/**
 * Unused as attribute index can be specified explicitly in shader (layout (location = index))
 * with index = `Attribute.id`
 */
GLuint Program::define_attribute(const std::string& attribute) const {
  // declared as const. bcos program passed by const reference in renderer
  return glGetAttribLocation(id, attribute.c_str());
}

void Program::set_uniforms(const Uniforms& uniforms) {
  // uniforms are passed via Mesh::draw() to program
  for (const auto& item : uniforms) {
    KeyUniform key_uniform(item.first);
    ValueUniform value_uniform(item.second);

    if (auto ptr_value = std::get_if<glm::mat4>(&value_uniform)) {
      set_mat4(key_uniform, *ptr_value);
    } else if (auto ptr_value = std::get_if<glm::vec3>(&value_uniform)) {
      set_vec3(key_uniform, *ptr_value);
    } else if (auto ptr_value = std::get_if<bool>(&value_uniform)) {
      set_bool(key_uniform, *ptr_value);
    } else if (auto ptr_value = std::get_if<int>(&value_uniform)) {
      set_int(key_uniform, *ptr_value);
    } else if (auto ptr_value = std::get_if<float>(&value_uniform)) {
      set_float(key_uniform, *ptr_value);
    } else if (auto ptr_value = std::get_if<Texture2D>(&value_uniform)) {
      ptr_value->attach();
      set_int(key_uniform, ptr_value->get_index());
    } else if (auto ptr_value = std::get_if<Texture3D>(&value_uniform)) {
      ptr_value->attach();
      set_int(key_uniform, ptr_value->get_index());
    } else {
      std::cout << "Incompatible value type" << '\n';
    }
  }
}
