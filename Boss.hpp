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

    glm::uvec4 color[5] = {
        glm::uvec4(255.f, 255.f, 0.f, 255.f),  // core color
        glm::uvec4(0.f, 255.f, 255.f, 255.f),  // normal color
        glm::uvec4(255.f, 0.f, 255.f, 255.f),  // shooter color
        glm::uvec4(0.f, 0.f, 0.f, 255.f),       // defence color
        glm::uvec4(0xAA, 0xA9, 0xAD, 255.f)       // bomber color
    };

    int triangle_health[5] = {1, 1, 1, 3, 1};
};

struct Boss { 
    
};



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