#pragma once

#include <glm/glm.hpp>
#include "Drawer.hpp"

struct GameState; // forward declaration

struct Bullet {
    bool destroyed = false;

    virtual void draw(Drawer& drawer) = 0;
    virtual void update(float elapsed, GameState& state) = 0;
};

struct CoreBullet : Bullet {
    glm::vec2 pos = {0.f, 0.f};
    glm::vec2 speed = {0.f, 0.f};

    CoreBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};

struct TurretBullet : Bullet {
    glm::vec2 pos = {0.f, 0.f};
    glm::vec2 speed = {0.f, 0.f};

    TurretBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};

struct ShooterBullet : Bullet {
    glm::vec2 pos;
    glm::vec2 speed;

    ShooterBullet(glm::vec2 pos, glm::vec2 speed);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};