#pragma once

#include "TriangleCluster.hpp"
#include <SDL.h>
#include "Player.hpp"
#include "DrawText.hpp"
#include "Controls.hpp"

struct Builder {
    Builder();
    Builder(int money);
	bool handle_event(SDL_Event const &, glm::uvec2 const &window_size);
	Player* update(float elapsed);
	void draw(glm::uvec2 const &drawable_size);

    private:
    int remaining_money;
    DrawText TextRenderer;
    Player player;
    Controls controls;
    glm::vec2 drawer_min;
    glm::vec2 drawer_max;
    glm::vec2 window_min;
    glm::vec2 window_max;
    int menu_hover = -1;
    int menu_selected = -1;
    bool building_hovered = false;
    bool done_hovered = false;
    bool disconnected = false;
    std::pair<int,int> building_hover;
    glm::vec2 center = {0.f, 0.f};
    
    pair<glm::vec2, glm::vec2> get_menu_item_bounds(int idx);
    pair<int, int> get_triangle_coords(glm::vec2 drawer_coords);
    pair<glm::vec2, glm::vec2> get_finished_button_bounds();
};
