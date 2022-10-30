#pragma once

#include "TriangleCluster.hpp"

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

TriangleCluster::TriangleCluster() {
    pos = {0.f, 0.f};
    angle = 0.0f;
    size = 1;
}

void TriangleCluster::insertTriangle(int i, int j) {
    assert(!triangles.count({i, j}));
    triangles.insert({i, j});
}

void TriangleCluster::eraseTriangle(int i, int j) {
    //assert(triangles.count({i,j}));
    if (i != 0 || j != 0) {
        triangles.erase({i,j});
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
