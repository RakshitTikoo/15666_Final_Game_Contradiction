#pragma once

#include <glm/glm.hpp>
#include <set>
#include <vector>

struct TriangleCluster {
    // "coordinates" are defined like so: https://imgur.com/a/ekUSVPs
    // even x means right-side-up, odd means upside-down

    glm::vec2 pos; // position of middle of (0,0) triangle
    float angle; // angle of rotation of the whole cluster
    float size; // side length of one triangle
    std::set<std::pair<int, int>> triangles; // coordinates -> Triangle
    //std::set<uint8_t> type; // 0 - basic , 1 - shooter_1, 2 - defence, 3 - shooter_2 , 4 - bomb/etc
    TriangleCluster();

    void insertTriangle(int i, int j);
    void eraseTriangle(int i, int j);

    // center of (i,j) triangle
    glm::vec2 getTrianglePosition(int i, int j);

    std::vector<glm::vec2> getTriangleCorners(int i, int j);
};
