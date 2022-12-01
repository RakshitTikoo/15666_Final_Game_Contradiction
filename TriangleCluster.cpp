#include "TriangleCluster.hpp"

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

TriangleCluster::TriangleCluster() {
    pos = {0.f, 0.f};
    angle = 0.0f;
    size = 1;
    bbox_mins = {1e9, 1e9};
    bbox_maxes = {-1e9, -1e9};
}

void TriangleCluster::insertTriangle(int i, int j) {
    assert(!triangles.count({i, j}));
    triangles.insert({i, j});

    // Update bbox
    glm::vec2 added = glm::rotate(getTrianglePosition(i,j) - pos, glm::radians(-angle));
    bbox_mins.x = fmin(bbox_mins.x, added.x - size);
    bbox_mins.y = fmin(bbox_mins.y, added.y - size);
    bbox_maxes.x = fmax(bbox_maxes.x, added.x + size);
    bbox_maxes.y = fmax(bbox_maxes.y, added.y + size);
}

void TriangleCluster::eraseTriangle(int i, int j) {
    assert(triangles.count({i,j}));
    triangles.erase({i,j});

    // Update bbox every so often
    static int calls_since_bbox_update = 0;
    calls_since_bbox_update++;
    if (calls_since_bbox_update > 10) {
        calls_since_bbox_update = 0;
        bbox_mins = {1e9, 1e9};
        bbox_maxes = {-1e9, -1e9};
        for (std::pair<int,int> t : triangles) {
            glm::vec2 tpos = glm::rotate(getTrianglePosition(t.first, t.second) - pos, glm::radians(-angle));
            bbox_mins.x = fmin(bbox_mins.x, tpos.x - size);
            bbox_mins.y = fmin(bbox_mins.y, tpos.y - size);
            bbox_maxes.x = fmax(bbox_maxes.x, tpos.x + size);
            bbox_maxes.y = fmax(bbox_maxes.y, tpos.y + size);
        }
    }
}

glm::vec2 TriangleCluster::getTrianglePosition(int i, int j) {
    static float height = size * sqrtf(3.f) / 2;
    static float len1 = size * sqrtf(3.f) / 6; // base to middle

    float offset_x = (i + j) * size / 2.0f;
    float offset_y = j * height + ((i % 2 == 0) ? 0.0f : len1);

    glm::vec2 offset = glm::vec2(offset_x, offset_y);
    offset = glm::rotate(offset, glm::radians(this->angle));

    return (this->pos + offset);
}

std::vector<glm::vec2> TriangleCluster::getTriangleCorners(int i, int j) {
    static float len1 = size * sqrtf(3.f) / 6; // base to middle
    static float len2 = size * sqrtf(3.f) / 3; // corner to middle

    glm::vec2 center = getTrianglePosition(i,j);

    std::vector<glm::vec2> res;
    if (i%2 == 0) { // right-side-up
        res.push_back(center + glm::rotate(glm::vec2(-size/2.f, -len1), glm::radians(this->angle)));
        res.push_back(center + glm::rotate(glm::vec2(size/2.f, -len1), glm::radians(this->angle)));
        res.push_back(center + glm::rotate(glm::vec2(0.f, len2), glm::radians(this->angle)));
    } else { // upside-down
        res.push_back(center + glm::rotate(glm::vec2(size/2.f, len1), glm::radians(this->angle)));
        res.push_back(center + glm::rotate(glm::vec2(-size/2.f, len1), glm::radians(this->angle)));
        res.push_back(center + glm::rotate(glm::vec2(0.f, -len2), glm::radians(this->angle)));
    }
    return res;
}

std::pair<int,int>* TriangleCluster::intersect(const Hitbox& hitbox) {
    // Check if it intersects our bbox first
    std::vector<glm::vec2> p = get_bbox_corners();
    TriangleHitbox h1 = TriangleHitbox(p[0], p[1], p[2]);
    TriangleHitbox h2 = TriangleHitbox(p[0], p[3], p[2]);
    if (!h1.intersect(hitbox) && !h2.intersect(hitbox)) return nullptr;

    for (std::pair<int,int> t : triangles) {
        std::vector<glm::vec2> corners = getTriangleCorners(t.first, t.second);
        TriangleHitbox tri_hitbox = TriangleHitbox(corners[0], corners[1], corners[2]);
        if (tri_hitbox.intersect(hitbox)) {
            return new std::pair(t.first, t.second);
        }
    }
    return nullptr;
}

std::vector<glm::vec2> TriangleCluster::get_bbox_corners() {
    glm::vec2 p0 = {bbox_mins.x, bbox_mins.y};
    glm::vec2 p1 = {bbox_maxes.x, bbox_mins.y};
    glm::vec2 p2 = {bbox_maxes.x, bbox_maxes.y};
    glm::vec2 p3 = {bbox_mins.x, bbox_maxes.y};
    std::vector<glm::vec2> res;
    res.push_back(glm::rotate(p0, glm::radians(angle)) + pos);
    res.push_back(glm::rotate(p1, glm::radians(angle)) + pos);
    res.push_back(glm::rotate(p2, glm::radians(angle)) + pos);
    res.push_back(glm::rotate(p3, glm::radians(angle)) + pos);
    return res;
}
