#pragma once

#include <map>
#include "TriangleCluster.hpp"
#include <glm/glm.hpp>
#include "Drawer.hpp"
#include "Controls.hpp"
#include "Hitbox.hpp"

#define size_map 34


struct GameState; // forward declaration

struct BossTriangle {
    int type; // 0 - core , 1 - basic, 2 - shooter1, 3 - defence
    int health; // 1 - by default, defence - 5

    glm::uvec4 color[7] = {
        glm::uvec4(255.f, 255.f, 0.f, 255.f),  // core color
        glm::uvec4(0.f, 255.f, 255.f, 255.f),  // normal color
        glm::uvec4(255.f, 0.f, 255.f, 255.f),  // shooter color
        glm::uvec4(0.f, 0.f, 0.f, 255.f),      // defence color
        glm::uvec4(0xAA, 0xA9, 0xAD, 255.f),   // bomber color
        glm::uvec4(50.f, 255.f, 50.f, 255.f),  // infector color
        glm::uvec4(255.f, 255.f, 255.f, 255.f) // timestop color
    };

    int triangle_health[7] = {1, 1, 1, 3, 1, 1, 1};
};

struct Boss { 
    float timestop_hit_cooldown = 10.f;
    float timestop_hit_cnt;
    bool timestop_hit = false;
};

// ==============
// TROJAN
// ==============

struct TrojanTriangle : BossTriangle { 
    TrojanTriangle(int type);
    TrojanTriangle();
};

struct Trojan : Boss { 
    Trojan(glm::vec2 pos);
    
    std::map<std::pair<int, int>, TrojanTriangle> triangle_info; // stores information for each triangle in the cluster
    TriangleCluster cluster; // stores the triangle cluster itself

    enum TriangleType {C = 0, N = 1, S = 2, D = 3, B = 4}; // C - Core, N - Normal, S - Shooter, D - Defence, B - Bomber 

    std::vector<std::pair<int, int>> triangle_coords;
    std::vector<int> triangle_type;

    
    int triangle_type_map[size_map][size_map] = 
                                    {{N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, B, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, B, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, C, B, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, B, B, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, B, B, B, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},};


    int triangle_coords_map[size_map][size_map] = 
                                    {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};



    void convert_map2_coordinates(int triangle_type_map_val[size_map][size_map], int triangle_coords_map_val[size_map][size_map]); 


    void draw(Drawer& drawer);
    void update(float elapsed, GameState& gs); // state 0 - rest | 1 - shooting mode 1 | 2 - shooting mode 2 | 3 - RAM Mode | 4 - Bomb Mode


    void addTriangle(int i, int j, TrojanTriangle t);
    void destroyTriangle(int i, int j);
    void destroyTriangles(std::vector<std::pair<int,int>> coords);
    void eraseSingleTriangle(int i, int j);
    void dfsEraseTriangles();
    void dfsEraseTrianglesInner(int i, int j, std::set<std::pair<int, int>>& visited);


    void check_triangle_collision(GameState &gs);
    void check_triangle_bomb_collision(GameState &gs);

    bool check_triangle_timestop_collision(GameState &gs);

    enum BossState {IDLE = 0, SHOOT1 = 1, SHOOT2 = 2, BOMB = 3, CHASE = 4};

    float alive_time = 0.f;

    float cluster_angle = 0.f;
    float mov_speed = 8.f;
    float chase_speed = 3.f;


    float dist_from_player = 10.f;



    float shoot1_rate = 1.f;
    float shoot1_cnt = 1.f;

    float shoot2_rate = 0.3f;
    float shoot2_cnt = 0.3f;

    float shoot2_dir_max = 20.f;
    float shoot2_dir_cnt = 0.f;
    int shoot2_dir = 0;

    float bomb_rate = 2.5f;
    float bomb_cnt = 2.5f;

    float bullet_speed1 = 20.f;
    float bullet_speed2 = 20.f;

    glm::vec2 core_loc();
    

};



// ================
// INFBOSS
// ================

struct InfbossTriangle : BossTriangle { 
    InfbossTriangle(int type);
    InfbossTriangle();
};

struct Infboss : Boss { 
    Infboss(glm::vec2 pos);
    
    std::map<std::pair<int, int>, InfbossTriangle> triangle_info; // stores information for each triangle in the cluster
    TriangleCluster cluster; // stores the triangle cluster itself

    enum TriangleType {C = 0, N = 1, S = 2, D = 3, B = 4, I = 5}; // C - Core, N - Normal, S - Shooter, D - Defence, B - Bomber, I = Infector 

    std::vector<std::pair<int, int>> triangle_coords;
    std::vector<int> triangle_type;

    
    int triangle_type_map[size_map][size_map] = 
                                    {{N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, B, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, B, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, C, B, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, B, B, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, B, B, B, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},};


    int triangle_coords_map[size_map][size_map] = 
                                    {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};



    void convert_map2_coordinates(int triangle_type_map_val[size_map][size_map], int triangle_coords_map_val[size_map][size_map]); 


    void draw(Drawer& drawer);
    void update(float elapsed, GameState& gs); // state 0 - rest | 1 - shooting mode 1 | 2 - shooting mode 2 | 3 - RAM Mode | 4 - Bomb Mode | 5 - Infector Mode


    void addTriangle(int i, int j, InfbossTriangle t);
    void destroyTriangle(int i, int j);
    void destroyTriangles(std::vector<std::pair<int,int>> coords);
    void eraseSingleTriangle(int i, int j);
    void dfsEraseTriangles();
    void dfsEraseTrianglesInner(int i, int j, std::set<std::pair<int, int>>& visited);


    void check_triangle_collision(GameState &gs);
    void check_triangle_bomb_collision(GameState &gs);

    bool check_triangle_timestop_collision(GameState &gs);

    enum BossState {IDLE = 0, SHOOT1 = 1, SHOOT2 = 2, BOMB = 3, CHASE = 4, INFECT = 5};

    float alive_time = 0.f;

    float cluster_angle = 0.f;
    float mov_speed = 8.f;
    float chase_speed = 3.f;


    float dist_from_player = 10.f;



    float shoot1_rate = 1.f;
    float shoot1_cnt = 1.f;

    float shoot2_rate = 1.0f;
    float shoot2_cnt = 1.0f;

    int shoot2_bullet_max = 20;
    int shoot2_bullet_cnt = 0;
    
    float shoot2_bullet_per_shot_rate = 0.1f;
    float shoot2_bullet_per_shot_cnt = 0.1f;


    float bullet_speed1 = 20.f;
    float bullet_speed2 = 20.f;

    //float shoot2_dir_max = 20.f;
    //float shoot2_dir_cnt = 0.f;
    //int shoot2_dir = 0;

    float bomb_rate = 2.5f;
    float bomb_cnt = 2.5f;


    float infect_rate = 2.5f;
    float infect_cnt = 2.5f;


    glm::vec2 core_loc();
    

};



// ================
// TIMESTOPBOSS
// ================

struct TimestopTriangle : BossTriangle { 
    TimestopTriangle(int type);
    TimestopTriangle();
};

struct Timestopboss : Boss { 
    Timestopboss(glm::vec2 pos);
    
    std::map<std::pair<int, int>, TimestopTriangle> triangle_info; // stores information for each triangle in the cluster
    TriangleCluster cluster; // stores the triangle cluster itself

    enum TriangleType {C = 0, N = 1, S = 2, D = 3, B = 4, I = 5, T = 6}; // C - Core, N - Normal, S - Shooter, D - Defence, B - Bomber, I = Infector, T = Timestop

    std::vector<std::pair<int, int>> triangle_coords;
    std::vector<int> triangle_type;

    
    int triangle_type_map[size_map][size_map] = 
                                    {{N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, T, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, T, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, T, D, I, D, D, D, B, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, T, D, I, D, D, D, B, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, T, D, I, D, D, C, B, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, T, D, I, D, D, B, B, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, T, D, I, D, B, B, B, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, T, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, T, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, I, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, I, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, S, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, B, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
                                     {N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, N, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},};


    int triangle_coords_map[size_map][size_map] = 
                                    {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};



    void convert_map2_coordinates(int triangle_type_map_val[size_map][size_map], int triangle_coords_map_val[size_map][size_map]); 


    void draw(Drawer& drawer);
    void update(float elapsed, GameState& gs); // state 0 - rest | 1 - shooting mode 1 | 2 - shooting mode 2 | 3 - RAM Mode | 4 - Bomb Mode | 5 - Infector Mode | 6 -Timestop Mode


    void addTriangle(int i, int j, TimestopTriangle t);
    void destroyTriangle(int i, int j);
    void destroyTriangles(std::vector<std::pair<int,int>> coords);
    void eraseSingleTriangle(int i, int j);
    void dfsEraseTriangles();
    void dfsEraseTrianglesInner(int i, int j, std::set<std::pair<int, int>>& visited);


    void check_triangle_collision(GameState &gs);
    void check_triangle_bomb_collision(GameState &gs);

    
    bool check_triangle_timestop_collision(GameState &gs);


    enum BossState {IDLE = 0, SHOOT1 = 1, SHOOT2 = 2, BOMB = 3, CHASE = 4, INFECT = 5, TIMESTOP = 6};

    float alive_time = 0.f;

    float cluster_angle = 0.f;
    float mov_speed = 8.f;
    float chase_speed = 3.f;


    float dist_from_player = 10.f;



    float shoot1_rate = 1.f;
    float shoot1_cnt = 1.f;

    float shoot2_rate = 1.0f;
    float shoot2_cnt = 1.0f;

    int shoot2_bullet_max = 20;
    int shoot2_bullet_cnt = 0;
    
    float shoot2_bullet_per_shot_rate = 0.1f;
    float shoot2_bullet_per_shot_cnt = 0.1f;

    glm::vec2 shoot2_dir_1 = glm::vec2( 1.f,  0.f);
    glm::vec2 shoot2_dir_2 = glm::vec2(-1.f,  0.f);
    glm::vec2 shoot2_dir_3 = glm::vec2( 0.f,  1.f);
    glm::vec2 shoot2_dir_4 = glm::vec2( 0.f, -1.f);


    float bullet_speed1 = 20.f;
    float bullet_speed2 = 20.f;

    //float shoot2_dir_max = 20.f;
    //float shoot2_dir_cnt = 0.f;
    //int shoot2_dir = 0;

    float bomb_rate = 2.5f;
    float bomb_cnt = 2.5f;


    float infect_rate = 2.5f;
    float infect_cnt = 2.5f;


    float timestop_rate = 7.5f;
    float timestop_cnt =  7.5f;

    float timestop_max_rad = 12.f;
    float timestop_speed = 0.25f;
    
    std::vector<glm::vec2> timestop_pos;
    std::vector<float> timestop_rad;
    
    void draw_timestop(Drawer& drawer);
    bool timestop_intersect(const Hitbox& hitbox);

    glm::vec2 core_loc();
    

};