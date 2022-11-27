#pragma once

#include <glm/glm.hpp>
#include "SDL.h"

struct Controls {
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
		uint8_t once = 0;
	} left, right, down, up, q, e, mouse, space, enter, escape, arrow_down, arrow_up, f;
	glm::vec2 mouse_loc;

	bool handle_event(SDL_Event const &evt, glm::uvec2 const &window_size);
};
