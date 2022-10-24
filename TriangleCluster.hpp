#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>

struct Triangle {
    glm::uvec4 color; // temporary, for testing purposes;
    // in reality, we should store properties like whether or not there's a turret, hp, etc.

    Triangle() {
        color = {255.f, 0.f, 255.f, 1.f}; // default purple
    }
};

struct TriangleCluster {
    // "coordinates" are defined like so: https://imgur.com/a/ekUSVPs
    // even x means right-side-up, odd means upside-down

    glm::vec2 pos; // position of middle of (0,0) triangle
    float size; // side length of one triangle
    std::map<std::pair<int, int>, Triangle> triangles; // coordinates -> Triangle

    TriangleCluster();

    void insertTriangle(int i, int j, Triangle t);

    void eraseTriangle(int i, int j);

    // center of (i,j) triangle
    glm::vec2 getTrianglePosition(int i, int j);

    std::vector<glm::vec2> getTriangleCorners(int i, int j);
};
