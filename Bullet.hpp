#pragma once

#include <glm/glm.hpp>
#include "Drawer.hpp"
#include "Hitbox.hpp"

struct GameState; // forward declaration

struct Bullet {
    bool destroyed = false;

    virtual void draw(Drawer& drawer) = 0;
    virtual void update(float elapsed, GameState& state) = 0;
};

struct CoreBullet : Bullet {
    glm::vec2 pos = {0.f, 0.f};
    glm::vec2 speed = {0.f, 0.f};
    float rad = 0.1f;

    CoreBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};

struct TurretBullet : Bullet {
    glm::vec2 pos = {0.f, 0.f};
    glm::vec2 speed = {0.f, 0.f};
    float rad = 0.1f;

    TurretBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};

struct ShooterBullet : Bullet {
    glm::vec2 pos;
    glm::vec2 speed;
    float rad = 0.1f;

    ShooterBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};
