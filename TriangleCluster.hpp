#pragma once

#include <glm/glm.hpp>
#include <set>
#include <vector>
#include <map>
#include "Hitbox.hpp"

struct TriangleCluster {
    // "coordinates" are defined like so: https://imgur.com/a/ekUSVPs
    // even x means right-side-up, odd means upside-down

    glm::vec2 pos = {0.f, 0.f}; // position of middle of (0,0) triangle
    float angle = 0.f; // angle of rotation of the whole cluster
    float size = 1.f; // side length of one triangle
    std::set<std::pair<int, int>> triangles; // coordinates -> Triangle
    
    TriangleCluster();

    void insertTriangle(int i, int j);
    void eraseTriangle(int i, int j);

    // center of (i,j) triangle
    glm::vec2 getTrianglePosition(int i, int j);

    std::vector<glm::vec2> getTriangleCorners(int i, int j);

    // nullptr if no intersect, otherwise the coords of a triangle that touches hitbox
    std::pair<int,int>* intersect(const Hitbox& hitbox);
};
