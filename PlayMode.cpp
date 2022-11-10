#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/norm.hpp>

#include <random>
#include <cmath>
#include <iostream>

#include "GeoHelpers.hpp"
#include "Drawer.hpp"

#define WIDTH 960
#define HEIGHT 540

//compiles+links an OpenGL shader program from source.
// throws on compilation error.
GLuint gl_compile_program(std::string const &vertex_shader_source,std::string const &fragment_shader_source);

void PlayMode::spawn_entity(int entity_type) {
	glm::vec2 pos = glm::vec2(rand01(), rand01()) * (gs.arena_max - gs.arena_min) + gs.arena_min;
	if (entity_type != FOOD) {
		// reroll until far enough away
		while (glm::length(pos - gs.player.cluster.pos) < 30) {
			pos = glm::vec2(rand01(), rand01()) * (gs.arena_max - gs.arena_min) + gs.arena_min;
		}
	}

	switch(entity_type) {
		case FOOD:
			gs.food.push_back(pos);
			break;

		case CHASER:
			gs.enemies.push_back(new Chaser(pos));
			break;
		
		case SHOOTER:
			gs.enemies.push_back(new Shooter(pos));
			break;

		case SPIRAL:
			gs.enemies.push_back(new Spiral(pos));
			break;
		
		case BOMBER:
			gs.enemies.push_back(new Bomber(pos));
			break;
		
		case TROJAN:
			gs.trojan = new Trojan(pos);
			break;

		default: 
			printf("\nError: Unknown Entity Type\n");
			break;
	}
}

void PlayMode::spawn_entities(int count, int entity_type) {
	for (int i = 0; i < count; i++) {
		spawn_entity(entity_type);
	}
}

void PlayMode::init(){
	gs.player = Player();
	gs.bullets.clear();
	gs.food.clear();
	gs.enemies.clear();
	gs.state = 0;
	gs.score = 0;
	gs.trojan = nullptr;

	gs.current_level = 0;
	gs.current_wave = -1; // hacky way to get first wave to spawn on update
}

PlayMode::PlayMode() {
	init();
	std::cout << "Initialization successful\n"; 
	
	gs.MainLoop = Sound::loop(*gs.main_music, gs.main_volume, 0.0f);
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_a) {
			controls.left.downs += 1;
			controls.left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			controls.right.downs += 1;
			controls.right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			controls.up.downs += 1;
			controls.up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			controls.down.downs += 1;
			controls.down.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_e) {
			controls.q.downs += 1;
			controls.q.pressed = true;
		} else if (evt.key.keysym.sym == SDLK_q) {
			controls.e.downs += 1;
			controls.e.pressed = true;
		} else if (evt.key.keysym.sym == SDLK_SPACE && controls.space.once == 0) {
			controls.space.downs += 1;
			controls.space.pressed = true;
			controls.space.once = 1;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			controls.left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			controls.right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			controls.up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			controls.down.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_e) {
			controls.q.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_q) {
			controls.e.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			controls.space.pressed = false;
			controls.space.once = 0;
			return true;
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		controls.mouse.downs += 1;
		controls.mouse.pressed = true;
		return true;
	} else if (evt.type == SDL_MOUSEBUTTONUP) {
		controls.mouse.pressed = false;
		return true;
	}

	int x, y; SDL_GetMouseState(&x, &y);
	controls.mouse_loc = glm::vec2();
	controls.mouse_loc.x = x - float(window_size.x) / 2.f;
	controls.mouse_loc.y = float(window_size.y) / 2.f - y;

	return false;
}

void PlayMode::update(float elapsed) {
	if (gs.state == 0) {
		if(controls.space.pressed) {
			gs.state = 1;
		}
		return;
	}

	{ // update player
		gs.player.update(elapsed, gs, controls);
	}

	{ // update boss
		if (gs.trojan != nullptr) {
			gs.trojan->update(elapsed, gs);
		}
	}

	{ // update enemies
		for (Enemy* e : gs.enemies) {
			e->update(elapsed, gs);
		}
	}

	{ // update bullets
		for (Bullet* b : gs.bullets) {
			b->update(elapsed, gs);
		}
	}

	{ // destroy dead things
		for (int i = (int)gs.enemies.size()-1; i >= 0; i--) {
			if (gs.enemies[i]->destroyed) {
				gs.enemies.erase(gs.enemies.begin() + i);
				gs.score += 1;
			}
		}
		for (int i = (int)gs.bullets.size()-1; i >= 0; i--) {
			if (gs.bullets[i]->destroyed) {
				gs.bullets.erase(gs.bullets.begin() + i);
			}
		}
		if (gs.player.cluster.triangles.size() == 0) {
			Sound::play(*gs.player_destroyed, 3.0f*gs.sound_effect_volume, 0.0f);
			init();
		}
	}

	{ // Update wave
		if (gs.enemies.empty() && gs.trojan == nullptr) {
			gs.current_wave++;
			if (gs.current_wave == levels[gs.current_level].size()) {
				gs.current_level++;
				gs.current_wave = 0;
			}
			if (gs.current_level == NUM_LEVELS) {
				gs.state = 0;
				return;
			}
			// Spawn wave
			spawn_entities(MAX_FOOD - (int)gs.food.size(), FOOD);
			for (pair<int, int> to_spawn : levels[gs.current_level][gs.current_wave]) {
				spawn_entities(to_spawn.first, to_spawn.second);
			}
		}
	}

	//reset button press counters:
	{
		controls.left.downs = 0;
		controls.right.downs = 0;
		controls.up.downs = 0;
		controls.down.downs = 0;
		controls.q.downs = 0;
		controls.e.downs = 0;
		controls.mouse.downs = 0;
		controls.space.pressed = 0;
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	Drawer drawer(drawable_size);
	drawer.set_center(gs.player.cluster.pos);
	drawer.set_width(40.f);

	{ // draw the player
		gs.player.draw(drawer);
	}

	{ // draw boss
		if (gs.trojan != nullptr) {
			gs.trojan->draw(drawer);
		}
	}

	{ // draw player explosion
		gs.player.draw_explosion(drawer);

	}

	{ // draw food
		for (auto& k : gs.food) {
			drawer.circle(k, 0.1f, glm::u8vec4(0x00, 0xff, 0x00, 0xff));
		}
	}

	{ // draw basic_enemys
		for (Enemy* e : gs.enemies) {
			e->draw(drawer);
		}
	}

	{ // draw bullets
		for (Bullet* b : gs.bullets) {
			b->draw(drawer);
		}
	}

	{ // draw bounds
		glm::u8vec4 color = {255.f, 255.f, 255.f, 255.f};
		drawer.line(glm::vec2(gs.arena_min.x, gs.arena_min.y),
					glm::vec2(gs.arena_min.x, gs.arena_max.y),
					color);
		drawer.line(glm::vec2(gs.arena_min.x, gs.arena_max.y),
					glm::vec2(gs.arena_max.x, gs.arena_max.y),
					color);
		drawer.line(glm::vec2(gs.arena_max.x, gs.arena_max.y),
					glm::vec2(gs.arena_max.x, gs.arena_min.y),
					color);
		drawer.line(glm::vec2(gs.arena_max.x, gs.arena_min.y),
					glm::vec2(gs.arena_min.x, gs.arena_min.y),
					color);
	}

	if (gs.state == 0) {
		drawer.text("Press space to begin", glm::vec2(-0.4f, 0.1f), 0.1f);
	} else {
		drawer.text("Wave " + std::to_string(gs.current_wave), glm::vec2(1.5f, 0.85f), 0.05f);
	}

	GL_ERRORS();
}
