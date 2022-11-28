#include "Controls.hpp"

bool Controls::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.downs += 1;
			down.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_e) {
			q.downs += 1;
			q.pressed = true;
		} else if (evt.key.keysym.sym == SDLK_q) {
			e.downs += 1;
			e.pressed = true;
		} else if (evt.key.keysym.sym == SDLK_SPACE && space.once == 0) {
			space.downs += 1;
			space.pressed = true;
			space.once = 1;
		} else if (evt.key.keysym.sym == SDLK_RETURN && enter.once == 0) {
			enter.downs += 1;
			enter.pressed = true;
			enter.once = 1;
		}
		else if (evt.key.keysym.sym == SDLK_UP && arrow_up.once == 0) {
			arrow_up.downs += 1;
			arrow_up.pressed = true;
			arrow_up.once = 1;
		}
		else if (evt.key.keysym.sym == SDLK_DOWN && arrow_down.once == 0) {
			arrow_down.downs += 1;
			arrow_down.pressed = true;
			arrow_down.once = 1;
		}
		else if (evt.key.keysym.sym == SDLK_ESCAPE && escape.once == 0) {
			escape.downs += 1;
			escape.pressed = true;
			escape.once = 1;
		}
		else if (evt.key.keysym.sym == SDLK_f && f.once == 0) {
			f.downs += 1;
			f.pressed = true;
			f.once = 1;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_e) {
			q.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_q) {
			e.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.pressed = false;
			space.once = 0;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RETURN) {
			enter.pressed = false;
			enter.once = 0;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			arrow_up.pressed = false;
			arrow_up.once = 0;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			arrow_down.pressed = false;
			arrow_down.once = 0;
			return true;
		} else if (evt.key.keysym.sym == SDLK_ESCAPE) {
			escape.pressed = false;
			escape.once = 0;
			return true;
		} else if (evt.key.keysym.sym == SDLK_f) {
			f.pressed = false;
			f.once = 0;
			return true;
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN && mouse.once == 0) {
		mouse.once = 1;
		mouse.downs += 1;
		mouse.pressed = true;
		return true;
	} else if (evt.type == SDL_MOUSEBUTTONUP) {
		mouse.once = 0; 
		mouse.pressed = false;
		return true;
	}

	int x, y;
	SDL_GetMouseState(&x, &y);
	mouse_loc = glm::vec2(x, window_size.y - y);
    return false;
}
