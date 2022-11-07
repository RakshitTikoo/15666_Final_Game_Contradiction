#include "Hitbox.hpp"
#include "GeoHelpers.hpp"

bool CircleCircleIntersect(const CircleHitbox& h1, const CircleHitbox& h2) {
    return glm::length(h1.pos - h2.pos) <= h1.rad + h2.rad;
}

bool CircleTriangleIntersect(const CircleHitbox& h1, const TriangleHitbox& h2) {
    if (GeoHelpers::pointInTriangle(h1.pos, h2.p0, h2.p1, h2.p2)) return true;
    if (GeoHelpers::pointToSegmentDistance(h1.pos, h2.p0, h2.p1) <= h1.rad) return true;
    if (GeoHelpers::pointToSegmentDistance(h1.pos, h2.p1, h2.p2) <= h1.rad) return true;
    if (GeoHelpers::pointToSegmentDistance(h1.pos, h2.p2, h2.p0) <= h1.rad) return true;
    return false;
}

bool TriangleTriangleIntersect(const TriangleHitbox& h1, const TriangleHitbox& h2) {
    // currently unimplemented
    return false;
}

CircleHitbox::CircleHitbox() {
    this->pos = {0.f, 0.f};
    this->rad = 0.f;
}

CircleHitbox::CircleHitbox(glm::vec2 pos, float rad) {
    this->pos = pos;
    this->rad = rad;
}

bool CircleHitbox::intersect(const Hitbox& other) const {
    return other.intersectWith(*this);
}

bool CircleHitbox::intersectWith(const CircleHitbox& other) const {
    return CircleCircleIntersect(*this, other);
}

bool CircleHitbox::intersectWith(const TriangleHitbox& other) const {
    return CircleTriangleIntersect(*this, other);
}

TriangleHitbox::TriangleHitbox() {
    this->p0 = {0.f, 0.f};
    this->p1 = {0.f, 0.f};
    this->p2 = {0.f, 0.f};
}

TriangleHitbox::TriangleHitbox(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2) {
    this->p0 = p0;
    this->p1 = p1;
    this->p2 = p2;
}

bool TriangleHitbox::intersect(const Hitbox& other) const {
    return other.intersectWith(*this);
}

bool TriangleHitbox::intersectWith(const CircleHitbox& other) const {
    return CircleTriangleIntersect(other, *this);
}

bool TriangleHitbox::intersectWith(const TriangleHitbox& other) const {
    return TriangleTriangleIntersect(*this, other);
}
