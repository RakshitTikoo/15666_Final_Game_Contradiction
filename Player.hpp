#pragma once

#include <map>
#include "TriangleCluster.hpp"
#include <glm/glm.hpp>

struct PlayerTriangle {
    glm::uvec4 color = glm::uvec4(255.f, 255.f, 0.f, 1.f);
};

struct Player {
    std::map<std::pair<int, int>, PlayerTriangle> triangle_info; // stores information for each triangle in the cluster
    TriangleCluster cluster; // stores the triangle cluster itself

    Player();

    void addTriangle(int i, int j, PlayerTriangle t);
    void destroyTriangle(int i, int j);
    void destroyTriangles(std::vector<std::pair<int,int>> coords);

private:
    void eraseSingleTriangle(int i, int j);
    void dfsEraseTriangles();
    void dfsEraseTrianglesInner(int i, int j, std::set<std::pair<int, int>>& visited);
};