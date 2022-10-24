#pragma once

#include <glm/glm.hpp>

namespace GeoHelpers{
    float cross(glm::vec2 v1, glm::vec2 v2);

    bool sameSide(glm::vec2 p1, glm::vec2 p2, glm::vec2 a, glm::vec2 b);

    bool pointInTriangle(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c);

    float pointToSegmentDistance(glm::vec2 p, glm::vec2 a, glm::vec2 b);
}