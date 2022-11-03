#pragma once

#include "Drawer.hpp"
#include <glm/glm.hpp>

struct GameState; // forward declaration

struct Enemy {
    virtual void draw(Drawer& drawer) = 0;
    virtual void update(float elapsed, GameState& state) = 0;
};

struct Chaser : Enemy {
    glm::vec2 pos = {0.f, 0.f};

    Chaser(glm::vec2 pos);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};

struct Shooter : Enemy {
    glm::vec2 pos = {0.f, 0.f};
    float bullet_cooldown = 0.f;

    Shooter(glm::vec2 pos);

    void draw(Drawer& drawer) override;
    void update(float elapsed, GameState& state) override;
};
