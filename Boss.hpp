#pragma once

#include <map>
#include "TriangleCluster.hpp"
#include <glm/glm.hpp>
#include "Drawer.hpp"
#include "Controls.hpp"
#include "Hitbox.hpp"

struct GameState; // forward declaration

struct BossTriangle {
    int type; // 0 - core , 1 - basic, 2 - shooter1, 3 - defence
    int health; // 1 - by default, defence - 5

    glm::uvec4 color[4] = {
        glm::uvec4(255.f, 255.f, 0.f, 255.f),  // core color
        glm::uvec4(0.f, 255.f, 255.f, 255.f),  // basic color
        glm::uvec4(255.f, 0.f, 255.f, 255.f),  // shooter1 color
        glm::uvec4(0.f, 0.f, 0.f, 255.f)       // defence color
    };

    int triangle_health[4] = {1, 1, 1, 3};
};

struct Boss { 
    
};



struct TrojanTriangle : BossTriangle { 
    TrojanTriangle(int type);
    TrojanTriangle();
};

struct Trojan : Boss { 
    Trojan();
    
    std::map<std::pair<int, int>, TrojanTriangle> triangle_info; // stores information for each triangle in the cluster
    TriangleCluster cluster; // stores the triangle cluster itself

    enum TriangleType {CORE = 0, BASIC = 1, SHOOT1 = 2, DEFENCE = 3, BOMB = 4};

    std::pair<int, int> triangle_coords[3] = {{0, 0}, {1, 1}, {2, 2}};
    int triangle_type[3] = {CORE, DEFENCE, DEFENCE};

    void draw(Drawer& drawer);
    void update(float elapsed, GameState& gs, int state); // state 0 - rest | 1 - shooting mode 1 | 2 - shooting mode 2 | 3 - RAM Mode | 4 - Bomb Mode


    void addTriangle(int i, int j, TrojanTriangle t);
    void destroyTriangle(int i, int j);
    void destroyTriangles(std::vector<std::pair<int,int>> coords);
    void eraseSingleTriangle(int i, int j);
    void dfsEraseTriangles();
    void dfsEraseTrianglesInner(int i, int j, std::set<std::pair<int, int>>& visited);


};