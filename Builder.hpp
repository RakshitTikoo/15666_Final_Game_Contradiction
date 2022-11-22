#pragma once

#include "TriangleCluster.hpp"
#include <SDL.h>
#include "Player.hpp"

struct Builder {
    Builder();
    Builder(int money);
	bool handle_event(SDL_Event const &, glm::uvec2 const &window_size);
	Player* update(float elapsed);
	void draw(glm::uvec2 const &drawable_size);

    int remaining_money;
};
