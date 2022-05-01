#ifndef UNIFORMS_HPP
#define UNIFORMS_HPP

#include <variant>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "texture_2d.hpp"
#include "texture_3d.hpp"

// dictionary type for heteregenous uniform values
using KeyUniform = std::string;
using ValueUniform = std::variant<
  glm::mat4,
  glm::vec3,
  bool,
  int,
  float,
  Texture2D,
  Texture3D
>;
using Uniforms = std::unordered_map<KeyUniform, ValueUniform>;

#endif // UNIFORMS_HPP
