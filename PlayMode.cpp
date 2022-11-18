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

	// Init Text Renderer
	TextRenderer = DrawText("NotoSansMono_Condensed-Regular.ttf");


	std::cout << "Initialization successful\n"; 
	
	
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
		} else if (evt.key.keysym.sym == SDLK_RETURN && controls.enter.once == 0) {
			controls.enter.downs += 1;
			controls.enter.pressed = true;
			controls.enter.once = 1;
		}
		else if (evt.key.keysym.sym == SDLK_UP && controls.arrow_up.once == 0) {
			controls.arrow_up.downs += 1;
			controls.arrow_up.pressed = true;
			controls.arrow_up.once = 1;
		}
		else if (evt.key.keysym.sym == SDLK_DOWN && controls.arrow_down.once == 0) {
			controls.arrow_down.downs += 1;
			controls.arrow_down.pressed = true;
			controls.arrow_down.once = 1;
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
		} else if (evt.key.keysym.sym == SDLK_RETURN) {
			controls.enter.pressed = false;
			controls.enter.once = 0;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			controls.arrow_up.pressed = false;
			controls.arrow_up.once = 0;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			controls.arrow_down.pressed = false;
			controls.arrow_down.once = 0;
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
		// =========================
		// Title Screen Settings
		// =========================

		if(controls.arrow_up.pressed && controls.arrow_up.once == 1) {
			controls.arrow_up.once = 2;
			selected_option -= 1;
		}
		
		if(controls.arrow_down.pressed && controls.arrow_down.once == 1) {
			controls.arrow_down.once = 2;
			selected_option += 1;
		}

		if(selected_option < 0) selected_option = 0;
		if(selected_option > 5) selected_option = 5;


		if (gs.MainLoop != nullptr){
			gs.MainLoop->volume = 0.f;
		}
		for(int i = 0; i < 6; i++) {
			if(i != selected_option) {
				title_options_color[i] = glm::vec3(0.5f, 0.5f, 0.5f);
				title_options_scale[i] = 0.5f;
			}
			else {
				title_options_color[i] = glm::vec3(1.0f, 1.0f, 1.0f);
				title_options_scale[i] = 0.75f;
			}
		}

		
		 
		if(controls.enter.pressed) {
			if(selected_option == 0) { // Level 1 select
				gs.state = 1;
				gs.MainLoop = Sound::loop(*gs.main_music, gs.main_volume, 0.0f);
			}
			

			if(selected_option == 5) { // Quit select
				exit(0);
			}

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
		controls.enter.pressed = 0;
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gs.state == 0) {
		TextRenderer.draw_msg("Poly Defence", 100.f, 650.f, 1.f, drawable_size, glm::vec3(1.0f, 1.0f, 1.0f));
		TextRenderer.draw_msg(title_options[0], 100.f, 500.f, title_options_scale[0], drawable_size, title_options_color[0]);
		TextRenderer.draw_msg(title_options[1], 100.f, 450.f, title_options_scale[1], drawable_size, title_options_color[1]);
		TextRenderer.draw_msg(title_options[2], 100.f, 400.f, title_options_scale[2], drawable_size, title_options_color[2]);
		TextRenderer.draw_msg(title_options[3], 100.f, 350.f, title_options_scale[3], drawable_size, title_options_color[3]);
		TextRenderer.draw_msg(title_options[4], 100.f, 300.f, title_options_scale[4], drawable_size, title_options_color[4]);
		TextRenderer.draw_msg(title_options[5], 100.f, 250.f, title_options_scale[5], drawable_size, title_options_color[5]);
	} else {
		TextRenderer.draw_msg("Wave " + std::to_string(gs.current_wave), 900.f, 650.f, 0.5f, drawable_size, glm::vec3(1.0f, 1.0f, 1.0f));
	

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
		
	}
	
	GL_ERRORS();
}
