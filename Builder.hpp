#pragma once

#include "TriangleCluster.hpp"
#include <SDL.h>
#include "Player.hpp"
#include "DrawText.hpp"

struct Builder {
    Builder();
    Builder(int money);
	bool handle_event(SDL_Event const &, glm::uvec2 const &window_size);
	Player* update(float elapsed);
	void draw(glm::uvec2 const &drawable_size);

    private:
    int remaining_money;
    DrawText TextRenderer;
    pair<glm::vec2, glm::vec2> get_menu_item_bounds(int idx);
};
