#pragma once

#include "TriangleCluster.hpp"

TriangleCluster::TriangleCluster() {
    pos = {0.f, 0.f};
    size = 1;
}

void TriangleCluster::insertTriangle(int i, int j, Triangle t) {
    assert(!triangles.count({i,j}));
    triangles[{i,j}] = t;
}

void TriangleCluster::eraseTriangle(int i, int j) {
    assert(triangles.count({i,j}));
    triangles.erase({i,j});
}

glm::vec2 TriangleCluster::getTrianglePosition(int i, int j) {
    static float height = size * sqrtf(3.f)/2;
    static float len1 = size * sqrtf(3.f)/6; // base to middle

    float res_x = pos.x + (i+j)*size/2;
    float res_y = pos.y + j*height + (i%2 ? len1 : 0.f);

    return glm::vec2(res_x, res_y);
}

std::vector<glm::vec2> TriangleCluster::getTriangleCorners(int i, int j) {
    static float len1 = size * sqrtf(3.f)/6; // base to middle
    static float len2 = size * sqrtf(3.f)/3; // corner to middle

    glm::vec2 center = getTrianglePosition(i,j);

    std::vector<glm::vec2> res;
    if (i%2 == 0) { // right-side-up
        res.push_back(center + glm::vec2(-size/2.f, -len1));
        res.push_back(center + glm::vec2(size/2.f, -len1));
        res.push_back(center + glm::vec2(0.f, len2));
    } else { // upside-down
        res.push_back(center + glm::vec2(-size/2.f, len1));
        res.push_back(center + glm::vec2(size/2.f, len1));
        res.push_back(center + glm::vec2(0.f, -len2));
    }
    return res;
}
