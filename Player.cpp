#pragma once

#include "Player.hpp"
#include <functional>
#include <vector>

Player::Player() {
    cluster = TriangleCluster();
}

void Player::addTriangle(int i, int j, PlayerTriangle t) {
    assert(!cluster.triangles.count({i, j}));
    cluster.insertTriangle(i, j);
    triangle_info[{i,j}] = t;
}

void Player::destroyTriangle(int i, int j) {
    eraseSingleTriangle(i, j);
    dfsEraseTriangles();
}

void Player::destroyTriangles(std::vector<std::pair<int,int>> coords) {
    for (std::pair<int,int> c : coords) {
        eraseSingleTriangle(c.first, c.second);
    }
    dfsEraseTriangles();
}

void Player::eraseSingleTriangle(int i, int j) {
    //assert(cluster.triangles.count({i, j}));
    cluster.eraseTriangle(i, j);
    triangle_info.erase({i, j});
}

void Player::dfsEraseTriangles() {
    std::function<void(int, int, std::set<std::pair<int, int>>)> do_dfs;
    std::set<std::pair<int, int>> visited;
    dfsEraseTrianglesInner(0, 0, visited);
    
    // delete triangles that are not visited
    std::vector<std::pair<int, int>> toErase;
    for (std::pair<int, int> t : cluster.triangles) {
        if (!visited.count(t)) toErase.push_back(t);
    }
    for (std::pair<int, int> t : toErase) eraseSingleTriangle(t.first, t.second);
}

void Player::dfsEraseTrianglesInner(int i, int j, std::set<std::pair<int, int>>& visited) {
    if (!cluster.triangles.count({i, j})) return;
    if (visited.count({i, j})) return;
    
    visited.insert({i, j});

    if (i % 2 == 0) {
        dfsEraseTrianglesInner(i + 1, j, visited);
        dfsEraseTrianglesInner(i + 1, j - 1, visited);
        dfsEraseTrianglesInner(i - 1, j, visited);
    } else {
        dfsEraseTrianglesInner(i + 1, j, visited);
        dfsEraseTrianglesInner(i - 1, j + 1, visited);
        dfsEraseTrianglesInner(i - 1, j, visited);
    }
}
