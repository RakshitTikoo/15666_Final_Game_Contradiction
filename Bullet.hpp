#pragma once

#include <glm/glm.hpp>
#include "Drawer.hpp"
#include "Hitbox.hpp"

struct GameState; // forward declaration
#define PI 3.1415f

struct Bullet {
    bool destroyed = false;

    float timestop_hit_cooldown = 10.f;
    float timestop_hit_cnt;
    bool timestop_hit = false;

    virtual void draw(Drawer& drawer) = 0;
    virtual void update(float elapsed, GameState& state) = 0;
};

struct CoreBullet : Bullet {
    glm::vec2 pos = {0.f, 0.f};
    glm::vec2 speed = {0.f, 0.f};
    float rad = 0.1f;
    float lifespan = 0.7f;
    glm::uvec4 color = glm::uvec4(255.f, 255.f, 0.f, 255.f);
    CoreBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};

struct TurretBullet : Bullet {
    glm::vec2 pos = {0.f, 0.f};
    glm::vec2 speed = {0.f, 0.f};
    float rad = 0.1f;
    float lifespan = 1.5f;
    glm::uvec4 color = glm::uvec4(255.f, 0.f, 255.f, 255.f);
    TurretBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};

struct ShooterBullet : Bullet {
    glm::vec2 pos;
    glm::vec2 speed;
    float rad = 0.1f;
    float lifespan = 1.5f;
    glm::uvec4 color = glm::uvec4(255.f, 127.f, 0.f, 255.f);
    ShooterBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};

struct SpiralBullet : Bullet {
    glm::vec2 pos;
    float speed;
    glm::vec2 init_angle;
    float rad = 0.1f;
    
    float angular_speed = PI/100.f;
    float radial_speed = 0.25f;

    float angle;
    float rot_rad = 1.f;
    glm::vec2 ori_pos;
    
    float lifespan = 4.f;
    glm::uvec4 color = glm::uvec4(203.f, 10.f, 127.f, 255.f);
    SpiralBullet(glm::vec2 pos, glm::vec2 init_angle, float speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};

struct TrojanBullet : Bullet {
    glm::vec2 pos;
    glm::vec2 speed;
    float rad = 0.1f;
    glm::uvec4 color = glm::uvec4(255.f, 0.f, 255.f, 255.f);
    TrojanBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};

struct InfbossBullet : Bullet {
    glm::vec2 pos;
    glm::vec2 speed;
    float rad = 0.1f;
    glm::uvec4 color = glm::uvec4(255.f, 0.f, 255.f, 255.f);
    InfbossBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};

struct InfbossSpiralBullet : Bullet {
    glm::vec2 pos;
    float speed;
    glm::vec2 init_angle;
    float rad = 0.1f;
    
    float angular_speed = PI/100.f;
    float radial_speed = 0.5f;

    float angle;
    float rot_rad = 1.f;
    glm::vec2 ori_pos;
    
    float lifespan = 8.f;
    glm::uvec4 color = glm::uvec4(255.f, 0.f, 255.f, 255.f);
    InfbossSpiralBullet(glm::vec2 pos, glm::vec2 init_angle, float speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};


struct TimestopBullet : Bullet {
    glm::vec2 pos;
    glm::vec2 speed;
    float rad = 0.1f;
    glm::uvec4 color = glm::uvec4(255.f, 0.f, 255.f, 255.f);
    TimestopBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};