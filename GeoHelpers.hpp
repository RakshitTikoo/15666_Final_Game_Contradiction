#pragma once

#include <glm/glm.hpp>

namespace GeoHelpers{
    float cross(glm::vec2 v1, glm::vec2 v2);

    bool sameSide(glm::vec2 p1, glm::vec2 p2, glm::vec2 a, glm::vec2 b);

    bool pointInTriangle(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c);

    float pointToSegmentDistance(glm::vec2 p, glm::vec2 a, glm::vec2 b);

    bool pointInCircle(glm::vec2 point, glm::vec2 circle, float radius);

    bool segmentsIntersect(glm::vec2 p0, glm::vec2 p1, glm::vec2 q0, glm::vec2 q1);
}