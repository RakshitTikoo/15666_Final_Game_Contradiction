#pragma once

#include <glm/glm.hpp>
#include <set>
#include <vector>
#include <map>
struct TriangleCluster {
    // "coordinates" are defined like so: https://imgur.com/a/ekUSVPs
    // even x means right-side-up, odd means upside-down

    glm::vec2 pos; // position of middle of (0,0) triangle
    float angle; // angle of rotation of the whole cluster
    float size; // side length of one triangle
    std::set<std::pair<int, int>> triangles; // coordinates -> Triangle
    std::map<std::pair<int, int> , int> triangle_type; // coords 2 type // 0 - core , 1 - basic, 2 - shooter1, 3 - defence , so on ...
    
    TriangleCluster();

    void insertTriangleType(int i, int j, int type);
    void insertTriangle(int i, int j);
    void eraseTriangle(int i, int j);

    // center of (i,j) triangle
    glm::vec2 getTrianglePosition(int i, int j);

    std::vector<glm::vec2> getTriangleCorners(int i, int j);
};
