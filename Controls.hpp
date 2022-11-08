#pragma once

#include <glm/glm.hpp>

struct Controls {
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
		uint8_t once = 0;
	} left, right, down, up, q, e, mouse, space;
	glm::vec2 mouse_loc;
};
