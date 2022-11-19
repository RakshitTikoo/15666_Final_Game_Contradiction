#pragma once

#include <map>
#include "TriangleCluster.hpp"
#include <glm/glm.hpp>
#include "Drawer.hpp"
#include "Controls.hpp"
#include "Hitbox.hpp"

struct GameState; // forward declaration

struct PlayerTriangle {
    static constexpr float SHOOT_COOLDOWN = 0.5f;
    int type; // 0 - core , 1 - basic, 2 - shooter1, 3 - defence, 4 - Infector, 5 - Game Freeze
    int health; // 1 - by default, defence - 3
    float time_since_shoot = SHOOT_COOLDOWN;

    enum TriangleType {CORE = 0, BASIC = 1, SHOOTER = 2, DEFENCE = 3, INFECTOR = 4};

    glm::uvec4 color[5] = {
        glm::uvec4(255.f, 255.f, 0.f, 255.f),  // core color
        glm::uvec4(0.f, 255.f, 255.f, 255.f),  // basic color
        glm::uvec4(255.f, 0.f, 255.f, 255.f),  // shooter1 color
        glm::uvec4(128.f, 128.f, 128.f, 255.f),  // defence color
        glm::uvec4(50.f, 255.f, 50.f, 255.f) // Infector 
        //glm::uvec4(128.f, 128.f, 128.f, 255.f) // Time freeze
    };

    int triangle_health[5] = {1, 1, 1, 3, 1};

    PlayerTriangle();

    PlayerTriangle(int type);
};

struct Player {
    static constexpr float SHOOT_COOLDOWN = .25f;
    std::map<std::pair<int, int>, PlayerTriangle> triangle_info; // stores information for each triangle in the cluster
    TriangleCluster cluster; // stores the triangle cluster itself
    float time_since_shoot = SHOOT_COOLDOWN;
	float player_speed = 9.f;
	float player_rot = 300.f;
    float core_bullet_speed = 20.f;
    float turret_bullet_speed = 20.f;
    Player();

    void draw(Drawer& drawer);


    // Explosion related functions
    void draw_explosion(Drawer& drawer);
    bool explosion_intersect(const Hitbox& hitbox);

    float explosion_speed = 0.5f;
    std::vector<glm::vec2> explosion_pos;
    std::vector<float> explosion_rad;
    std::vector<float> explosion_max_rad; 

    void update(float elapsed, GameState& state, Controls& controls);

    void addTriangle(int i, int j, PlayerTriangle t);
    void destroyTriangle(int i, int j);
    void destroyTriangles(std::vector<std::pair<int,int>> coords);
    void eraseSingleTriangle(int i, int j);
    void dfsEraseTriangles();
    void dfsEraseTrianglesInner(int i, int j, std::set<std::pair<int, int>>& visited);
};