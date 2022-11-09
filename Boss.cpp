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


void Trojan::convert_map2_coordinates(int triangle_type_map_val[size_map][size_map], int triangle_coords_map_val[size_map][size_map]) {
    for(int i = 0; i < size_map; i++){
        for(int j = 0; j < size_map; j++){
            if(triangle_coords_map_val[i][j] == 1) {// Place triangle here 
                triangle_coords.push_back({i - size_map/2, j - size_map/2});
                triangle_type.push_back(triangle_type_map_val[i][j]);
            }
        }
    }
}



Trojan::Trojan() {
    cluster = TriangleCluster();
    convert_map2_coordinates(triangle_type_map, triangle_coords_map);

    //Add triangles defined in the triangle_coords array
    for(int i = 0; i < (int) triangle_coords.size(); i++)
    {
        std::pair<int, int> k = triangle_coords[i];
        //printf("Adding triangle %d %d %d\n", k.first, k.second, triangle_type[i]);
        addTriangle(k.first, k.second, TrojanTriangle(triangle_type[i]));
    }
	 
    cluster.pos = glm::vec2(0.f, 0.f);
    cluster.angle = cluster_angle;
}


void Trojan::draw(Drawer& drawer) {
    for (std::pair<int,int> coords : cluster.triangles) {
        std::vector<glm::vec2> corners = cluster.getTriangleCorners(coords.first, coords.second);
        TrojanTriangle t = triangle_info[coords];

        //drawer.line(corners[0], corners[1], t.color[t.type]);
        //drawer.line(corners[1], corners[2], t.color[t.type]);
        //drawer.line(corners[2], corners[0], t.color[t.type]);

        // Unnecessary time waste code to draw smaller triangle 
        glm::vec2 offset_0 = (cluster.getTrianglePosition(coords.first, coords.second) - corners[0]) / glm::length(cluster.getTrianglePosition(coords.first, coords.second) - corners[0]);
        offset_0 = offset_0*0.05f;
        glm::vec2 offset_1 = (cluster.getTrianglePosition(coords.first, coords.second) - corners[1]) / glm::length(cluster.getTrianglePosition(coords.first, coords.second) - corners[1]);
        offset_1 = offset_1*0.05f;
        glm::vec2 offset_2 = (cluster.getTrianglePosition(coords.first, coords.second) - corners[2]) / glm::length(cluster.getTrianglePosition(coords.first, coords.second) - corners[2]);
        offset_2 = offset_2*0.05f;

        drawer.line(corners[0] + offset_0 , corners[1] + offset_1, t.color[t.type]);
        drawer.line(corners[1] + offset_1, corners[2] + offset_2, t.color[t.type]);
        drawer.line(corners[2] + offset_2, corners[0] + offset_0, t.color[t.type]);
    }
}

glm::vec2 Trojan::core_loc(){
    for (auto& k : triangle_info) if (k.second.type == C) { // Core
        
        return cluster.getTrianglePosition(k.first.first, k.first.second);
        break;
    }
    return cluster.pos;
}


void Trojan::update(float elapsed, GameState& gs, int state) {
    // =====================
    // Boss movement logic
    // =====================

    float px = gs.player.cluster.pos.x; 
    float py = gs.player.cluster.pos.y; 

    glm::vec2 core_pos = core_loc();

    if(core_pos.x < px) {
        cluster.pos.x += elapsed*mov_speed;
        core_pos = core_loc();
        if(core_pos.x > px) cluster.pos.x -= (core_pos.x - px);
    }

    if(core_pos.x > px) {
        cluster.pos.x -= elapsed*mov_speed;
        core_pos = core_loc();
       if(core_pos.x < px) cluster.pos.x += (px - core_pos.x);
    }

    core_pos = core_loc();

    if(std::abs(core_pos.y - py) < dist_from_player) {
        cluster.pos.y += elapsed*mov_speed;
        core_pos = core_loc();
        if(std::abs(core_pos.y - py) > dist_from_player) cluster.pos.y -= (std::abs((core_pos.y - py) - dist_from_player));
    }

    if(std::abs(core_pos.y - py) > dist_from_player) {
        cluster.pos.y -= elapsed*mov_speed;
        core_pos = core_loc();
        if(std::abs(core_pos.y - py) < dist_from_player) cluster.pos.y += (dist_from_player - std::abs((core_pos.y - py)));
    }

    // Bound Check
    if(cluster.pos.x > gs.arena_max.x) cluster.pos.x = gs.arena_max.x;
    if(cluster.pos.y > gs.arena_max.y) cluster.pos.y = gs.arena_max.y;
    if(cluster.pos.x < gs.arena_min.x) cluster.pos.x = gs.arena_min.x;
    if(cluster.pos.y < gs.arena_min.y) cluster.pos.y = gs.arena_min.y;

    // ================
    // State Update
    // ================

    if(state == IDLE) {
        return;
    }

    if(state == SHOOT1) {

    }

    if(state == SHOOT2) {

    }

    if(state == BOMB) {

    }

    if(state == CHARGE) {

    }

}

void Trojan::addTriangle(int i, int j, TrojanTriangle t) {
    assert(!cluster.triangles.count({i, j}));
    cluster.insertTriangle(i, j);
    triangle_info[{i,j}] = t;
}

void Trojan::destroyTriangle(int i, int j) {
    assert(cluster.triangles.count({i, j}));
    triangle_info[{i, j}].health -= 1;
    if(triangle_info[{i, j}].health <= 0)
    {
        eraseSingleTriangle(i, j);
        dfsEraseTriangles();
    }  
}

void Trojan::destroyTriangles(std::vector<std::pair<int,int>> coords) {
    for (std::pair<int,int> c : coords) {
        eraseSingleTriangle(c.first, c.second);
    }
    dfsEraseTriangles();
}

void Trojan::eraseSingleTriangle(int i, int j) {
    assert(cluster.triangles.count({i, j}));
    cluster.eraseTriangle(i, j);
    triangle_info.erase({i, j});
}

void Trojan::dfsEraseTriangles() {
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

void Trojan::dfsEraseTrianglesInner(int i, int j, std::set<std::pair<int, int>>& visited) {
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





