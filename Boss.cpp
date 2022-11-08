#pragma once

#include "Boss.hpp"
#include <functional>
#include <vector>
#include "GameState.hpp"
#include "GeoHelpers.hpp"
#include "Sound.hpp"

// ==================
// TROJAN
// ==================
TrojanTriangle::TrojanTriangle() {
    this->type = 0;
    this->health = triangle_health[0];
}

TrojanTriangle::TrojanTriangle(int type) {
    this->type = type;
    this->health = triangle_health[type];
}

Trojan::Trojan() {
    this->cluster = TriangleCluster();

    //Add triangles defined in the triangle_coords array
    for(int i = 0; i < (int) sizeof(triangle_coords)/sizeof(triangle_coords[0]); i++)
    {
        std::pair<int, int> k = triangle_coords[i];
        this->addTriangle(k.first, k.second, TrojanTriangle(triangle_type[i]));
    }
	 
    this->cluster.pos = glm::vec2(0.f, 0.f);
}


void Trojan::draw(Drawer& drawer) {
    for (std::pair<int,int> coords : this->cluster.triangles) {
        std::vector<glm::vec2> corners = this->cluster.getTriangleCorners(coords.first, coords.second);
        TrojanTriangle t = this->triangle_info[coords];

        //drawer.line(corners[0], corners[1], t.color[t.type]);
        //drawer.line(corners[1], corners[2], t.color[t.type]);
        //drawer.line(corners[2], corners[0], t.color[t.type]);

        // Unnecessary time waste code to draw smaller triangle 
        glm::vec2 offset_0 = (this->cluster.getTrianglePosition(coords.first, coords.second) - corners[0]) / glm::length(this->cluster.getTrianglePosition(coords.first, coords.second) - corners[0]);
        offset_0 = offset_0*0.05f;
        glm::vec2 offset_1 = (this->cluster.getTrianglePosition(coords.first, coords.second) - corners[1]) / glm::length(this->cluster.getTrianglePosition(coords.first, coords.second) - corners[1]);
        offset_1 = offset_1*0.05f;
        glm::vec2 offset_2 = (this->cluster.getTrianglePosition(coords.first, coords.second) - corners[2]) / glm::length(this->cluster.getTrianglePosition(coords.first, coords.second) - corners[2]);
        offset_2 = offset_2*0.05f;

        drawer.line(corners[0] + offset_0 , corners[1] + offset_1, t.color[t.type]);
        drawer.line(corners[1] + offset_1, corners[2] + offset_2, t.color[t.type]);
        drawer.line(corners[2] + offset_2, corners[0] + offset_0, t.color[t.type]);
    }
}

void Trojan::update(float elapsed, GameState& gs, int state) {
    printf("Updating");

    // Add action for each mode

    // Add collision

    // Add all boss defeat logic
}

void Trojan::addTriangle(int i, int j, TrojanTriangle t) {
    assert(!this->cluster.triangles.count({i, j}));
    this->cluster.insertTriangle(i, j);
    this->triangle_info[{i,j}] = t;
}

void Trojan::destroyTriangle(int i, int j) {
    assert(this->cluster.triangles.count({i, j}));
    this->triangle_info[{i, j}].health -= 1;
    if(this->triangle_info[{i, j}].health <= 0)
    {
        this->eraseSingleTriangle(i, j);
        this->dfsEraseTriangles();
    }  
}

void Trojan::destroyTriangles(std::vector<std::pair<int,int>> coords) {
    for (std::pair<int,int> c : coords) {
        this->eraseSingleTriangle(c.first, c.second);
    }
    this->dfsEraseTriangles();
}

void Trojan::eraseSingleTriangle(int i, int j) {
    assert(this->cluster.triangles.count({i, j}));
    this->cluster.eraseTriangle(i, j);
    this->triangle_info.erase({i, j});
}

void Trojan::dfsEraseTriangles() {
    std::function<void(int, int, std::set<std::pair<int, int>>)> do_dfs;
    std::set<std::pair<int, int>> visited;
    this->dfsEraseTrianglesInner(0, 0, visited);
    
    // delete triangles that are not visited
    std::vector<std::pair<int, int>> toErase;
    for (std::pair<int, int> t : this->cluster.triangles) {
        if (!visited.count(t)) toErase.push_back(t);
    }
    for (std::pair<int, int> t : toErase) this->eraseSingleTriangle(t.first, t.second);
}

void Trojan::dfsEraseTrianglesInner(int i, int j, std::set<std::pair<int, int>>& visited) {
    if (!this->cluster.triangles.count({i, j})) return;
    if (visited.count({i, j})) return;
    
    visited.insert({i, j});

    if (i % 2 == 0) {
        this->dfsEraseTrianglesInner(i + 1, j, visited);
        this->dfsEraseTrianglesInner(i + 1, j - 1, visited);
        this->dfsEraseTrianglesInner(i - 1, j, visited);
    } else {
        this->dfsEraseTrianglesInner(i + 1, j, visited);
        this->dfsEraseTrianglesInner(i - 1, j + 1, visited);
        this->dfsEraseTrianglesInner(i - 1, j, visited);
    }
}





