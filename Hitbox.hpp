#pragma once

#include <glm/glm.hpp>
#include "GeoHelpers.hpp"

// Based on https://stackoverflow.com/questions/70205523/c-is-there-a-more-elegant-solution-to-this-multiple-dispatch-runtime-polymo

struct CircleHitbox;
struct TriangleHitbox;

struct Hitbox {
    virtual bool intersect(const Hitbox&) const = 0;
    virtual bool intersectWith(const CircleHitbox&) const = 0;
    virtual bool intersectWith(const TriangleHitbox&) const = 0;
};

struct CircleHitbox : Hitbox {
    glm::vec2 pos = {0.f, 0.f};
    float rad = 0.f;

    CircleHitbox();

    CircleHitbox(glm::vec2 pos, float rad);

    bool intersect(const Hitbox& other) const override;

    bool intersectWith(const CircleHitbox& other) const override;

    bool intersectWith(const TriangleHitbox& other) const override;
};

struct TriangleHitbox : Hitbox {
    glm::vec2 p0 = {0.f, 0.f};
    glm::vec2 p1 = {0.f, 0.f};
    glm::vec2 p2 = {0.f, 0.f};

    TriangleHitbox();

    TriangleHitbox(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2);

    bool intersect(const Hitbox& other) const override;

    bool intersectWith(const CircleHitbox& other) const override;

    bool intersectWith(const TriangleHitbox& other) const override;
};
