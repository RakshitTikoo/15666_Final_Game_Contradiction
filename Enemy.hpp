#pragma once

#include "Drawer.hpp"
#include <glm/glm.hpp>
#include "Hitbox.hpp"

struct GameState; // forward declaration

struct Enemy {
    bool destroyed = false;

    virtual void draw(Drawer& drawer) = 0;
    virtual void update(float elapsed, GameState& state) = 0;
    virtual Hitbox* getHitbox() = 0;
};

struct Chaser : Enemy {
    glm::vec2 pos = {0.f, 0.f};
    float rad = 0.25f;
    float mov_speed = 8.f;
    glm::uvec4 color = glm::uvec4(255.f, 0.f, 0.f, 255.f);
    Chaser(glm::vec2 pos);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
    Hitbox* getHitbox() override;
};

struct Shooter : Enemy {
    glm::vec2 pos = {0.f, 0.f};
    float bullet_cooldown = 0.f;
    float rad = 0.5f;
    float mov_speed = 2.f;
    float bullet_speed = 20.f;
    glm::uvec4 color = glm::uvec4(255.f, 177.f, 0.f, 255.f);
    Shooter(glm::vec2 pos);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
    Hitbox* getHitbox() override;
};

struct Spiral : Enemy {
    glm::vec2 pos = {0.f, 0.f};
    
    float bullet_cooldown = 0.f;
    float bullet_per_shot_cooldown = 0.f;
    
    int total_bullets_shot = 0;
    int total_bullets_to_shoot = 20;

    float rad = 0.5f;
    float mov_speed = 1.f;
    float bullet_speed = 20.f;
    glm::uvec4 color = glm::uvec4(203.f, 10.f, 127.f, 255.f);
    Spiral(glm::vec2 pos);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
    Hitbox* getHitbox() override;
};


struct Bomber : Enemy {
    glm::vec2 pos = {0.f, 0.f};
    float rad = 0.25f;
    float bomb_rad = rad*20.f; 
    float mov_speed = 4.f;
    
    glm::uvec4 color = glm::uvec4(0xAA, 0xA9, 0xAD, 255.f);
    Bomber(glm::vec2 pos);

    int bomb_phase = 0;
    int blink_time = 75;

    int blink_render = 0;

    void draw_explosion();

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
    Hitbox* getHitbox() override;
};

struct Infector : Enemy {
    glm::vec2 pos = {0.f, 0.f};
    float rad = 0.25f;
    float mov_speed = 8.f;
    glm::uvec4 color = glm::uvec4(50.f, 255.f, 50.f, 255.f);
    Infector(glm::vec2 pos);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
    Hitbox* getHitbox() override;
};