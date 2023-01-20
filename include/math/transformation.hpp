#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include <glm/glm.hpp>
#include <array>

/**
 * Aggregator for transformation matrices (to avoid passing 3 params to `Renderer`)
 * Template non-type parameters (N_INSTANCES): As many model matrices (position) as there are instances
 */
template <size_t N_INSTANCES = 1>
struct Transformation {
  std::array<glm::mat4, N_INSTANCES> models;
  glm::mat4 view;
  glm::mat4 projection;

  Transformation() = default;
  Transformation(const std::array<glm::mat4, N_INSTANCES>& m, const glm::mat4& v, const glm::mat4& p);
};

#endif // TRANSFORMATION_HPP
