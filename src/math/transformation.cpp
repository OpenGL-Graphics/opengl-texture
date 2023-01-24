#include <boost/preprocessor/repetition/repeat_from_to.hpp>

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

// template instantiation using boost preprocessing (avoid linking error & repetition)
// https://stackoverflow.com/a/7399473/2228912
#define STRUCT(z, n, StructName) template struct StructName<n>;
BOOST_PP_REPEAT_FROM_TO(1, 4, STRUCT, Transformation)
