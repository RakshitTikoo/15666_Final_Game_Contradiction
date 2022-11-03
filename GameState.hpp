#pragma once

#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include <glm/glm.hpp>
#include <vector>

struct GameState {
	glm::vec2 arena_max = {50.f, 50.f};
	glm::vec2 arena_min = {-50.f, -50.f};

    Player player;
    std::vector<Enemy*> enemies;
    std::vector<Bullet*> bullets;
    std::vector<glm::vec2> food;

	uint32_t score = 0;
};
