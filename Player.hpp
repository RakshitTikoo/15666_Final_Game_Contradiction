#pragma once

#include <map>
#include "TriangleCluster.hpp"
#include <glm/glm.hpp>
#include "Drawer.hpp"
#include "Controls.hpp"
#include "Hitbox.hpp"
#include <string>

using namespace std;

struct GameState; // forward declaration

struct PlayerTriangle {
    static constexpr int NUM_TRIANGLE_TYPES = 5;
    struct TriangleTypeInfo {
        string name;
        glm::uvec4 color;
        int health;
        string description;
    };
    enum TriangleType : int {CORE, BASIC, SHOOTER, DEFENCE, INFECTOR};
    static inline TriangleTypeInfo triangleTypeMap[NUM_TRIANGLE_TYPES] = {
        {
            "Core",
            {255.f, 255.f, 0.f, 255.f},
            1,
            "The one you need to protect"
        },
        {
            "Basic",
            {0.f, 255.f, 255.f, 255.f},
            1,
            "Does nothing but can take one shot"
        },
        {
            "Shooter",
            {255.f, 0.f, 255.f, 255.f},
            1,
            "Shoots bullets where you aim"
        },
        {
            "Defense",
            {128.f, 128.f, 128.f, 255.f},
            3,
            "Takes 3 shots"
        },
        {
            "Infector",
            {50.f, 255.f, 50.f, 255.f},
            1,
            "Splits into 2 basic triangles once hit"
        }
    };

    static constexpr float SHOOT_COOLDOWN = 0.5f;
    int type;
    int health;
    float time_since_shoot = SHOOT_COOLDOWN;

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
    bool draw_bbox = false;
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