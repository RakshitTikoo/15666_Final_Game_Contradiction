#include "GeoHelpers.hpp"
#include <glm/gtx/norm.hpp>

namespace GeoHelpers{
    float cross(glm::vec2 v1, glm::vec2 v2) {
        return v1.x*v2.y - v1.y*v2.x;
    }

    bool sameSide(glm::vec2 p1, glm::vec2 p2, glm::vec2 a, glm::vec2 b) {
        float cp1 = cross(b-a, p1-a);
        float cp2 = cross(b-a, p2-a);
        if (cp1 * cp2 > 0) return true;
        return false;
    }

    // https://blackpawn.com/texts/pointinpoly/
    bool pointInTriangle(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c) {
        return sameSide(p,a, b,c) && sameSide(p,b, a,c) && sameSide(p,c, a,b);
    }

    // https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
    float pointToSegmentDistance(glm::vec2 p, glm::vec2 a, glm::vec2 b) {
        float l2 = glm::length2(a-b);
        if (l2 == 0.f) return glm::distance(p, a);
        float t = fmax(0.f, fmin(1.f, glm::dot(p-a, b-a)/l2));
        glm::vec2 proj = a + t * (b-a);
        return glm::distance(p, proj);
    }

    bool pointInCircle(glm::vec2 point, glm::vec2 circle, float radius) {
        if(glm::length(point - circle) < radius) return true;

        return false;
    };

    // https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
    bool segmentsIntersect(glm::vec2 p0, glm::vec2 p1, glm::vec2 q0, glm::vec2 q1) {
        glm::vec2 s1 = p1 - p0;
        glm::vec2 s2 = q1 - q0;

        float s = (-s1.y*(p0.x - q0.x) + s1.x*(p0.y - q0.y)) / (-s2.x*s1.y + s1.x*s2.y);
        float t = ( s2.x*(p0.y - q0.y) - s2.y*(p0.x - q0.x)) / (-s2.x*s1.y + s1.x*s2.y);

        if (0 <= s && s <= 1 && 0 <= t && t <= 1) return true;
        return false;
    }
}