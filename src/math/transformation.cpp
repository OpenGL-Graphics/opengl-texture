#include "math/transformation.hpp"

/**
 * Model matrix not needed as we'll never convert back to local space
 * Projection corresponds to projection 3D matrix (not 2D one)
 */
template <size_t N_INSTANCES>
Transformation<N_INSTANCES>::Transformation(const std::array<glm::mat4, N_INSTANCES>& ms, const glm::mat4& v, const glm::mat4& p):
  models(ms),
  view(v),
  projection(p)
{
}

// template instantiation to avoid linking error
// TODO: use boost preprocessing to loop over instantiations
template struct Transformation<1>;
template struct Transformation<2>;
template struct Transformation<3>;
