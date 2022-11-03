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

// Music Assets
Load< Sound::Sample > Main_Music(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("Main_Music.opus"));
});
Load< Sound::Sample > Player_Hit(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("player_hit.opus"));
});
Load< Sound::Sample > Player_Grow(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("player_grow.opus"));
});
Load< Sound::Sample > Player_Destroyed(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("player_destroyed.opus"));
});
Load< Sound::Sample > Player_Bullet(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("player_bullet.opus"));
});

void PlayMode::init(int state){
	this->begin_game = 0;

	if(state == 0) {
		gs.player = Player();
	}
	gs.player.addTriangle(0, 0, PlayerTriangle(0));

	for (int i = 0; i < 300; i++) {
		glm::vec2 pos = glm::vec2(rand01(), rand01()) * (gs.arena_max - gs.arena_min) + gs.arena_min;
		gs.food.push_back(pos);
	}

	for (int i = 0; i < 25; i++) {
		glm::vec2 pos = glm::vec2(rand01(), rand01()) * (gs.arena_max - gs.arena_min) + gs.arena_min;
		gs.enemies.push_back(new Chaser(pos));
	}

	for (int i = 0; i < 10; i++) {
		glm::vec2 pos = glm::vec2(rand01(), rand01()) * (gs.arena_max - gs.arena_min) + gs.arena_min;
		gs.enemies.push_back(new Shooter(pos));
	}
}

PlayMode::PlayMode() {
	init(0);
	std::cout << "Initialization successful\n"; 
	
	MainLoop = Sound::loop(*Main_Music, main_volume, 0.0f);
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
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
			rot_left.downs += 1;
			rot_left.pressed = true;
		} else if (evt.key.keysym.sym == SDLK_q) {
			rot_right.downs += 1;
			rot_right.pressed = true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.downs += 1;
			space.pressed = true;
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
			rot_left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_q) {
			rot_right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.pressed = false;
			return true;
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		mouse.downs += 1;
		mouse.pressed = true;
		return true;
	} else if (evt.type == SDL_MOUSEBUTTONUP) {
		mouse.pressed = false;
		return true;
	}

	int x, y; SDL_GetMouseState(&x, &y);
	mouse_loc = glm::vec2();
	mouse_loc.x = x - float(window_size.x) / 2.f;
	mouse_loc.y = float(window_size.y) / 2.f - y;

	return false;
}

void PlayMode::update(float elapsed) {
	if (begin_game == 0) {
		if(space.pressed) {
			begin_game = 1;
		}
	} else {
		// =================
		// game over logic
		// =================
		if (gs.player.cluster.triangles.size() == 0) {
			Sound::play(*Player_Destroyed, 3.0f*sound_effect_volume, 0.0f);
			init(1);
		}

		// ===================
		// player movement
		// ===================
		{
			// combine inputs into a move:
			glm::vec2 move = glm::vec2(0.0f);
			if (left.pressed && !right.pressed) move.x =-1.0f;
			if (!left.pressed && right.pressed) move.x = 1.0f;
			if (down.pressed && !up.pressed) move.y =-1.0f;
			if (!down.pressed && up.pressed) move.y = 1.0f;
			// make it so that moving diagonally doesn't go faster:
			if (move != glm::vec2(0.0f)) move = player_speed * glm::normalize(move) * elapsed;

			// Clip player inside arena
			gs.player.cluster.pos += move;
			gs.player.cluster.pos = max(gs.arena_min, gs.player.cluster.pos);
			gs.player.cluster.pos = min(gs.arena_max, gs.player.cluster.pos);

			if (rot_left.pressed) {
				gs.player.cluster.angle -= player_rot * elapsed;
				gs.player.cluster.angle = std::fmodf(gs.player.cluster.angle + 360.0f, 360.0f);
			}
			if (rot_right.pressed) {
				gs.player.cluster.angle += player_rot * elapsed;
				gs.player.cluster.angle = std::fmodf(gs.player.cluster.angle + 360.0f, 360.0f);
			}
		}

		// ==============================
		// eat food = grow a triangle
		// ==============================
		{
			std::vector<int> toErase;
			std::vector<std::pair<std::pair<int, int>, PlayerTriangle>> toInsert;
			for (int i = 0; i < (int)gs.food.size(); i++) {
				glm::vec2 foodpos = gs.food[i];
				for (std::pair<int,int> coords : gs.player.cluster.triangles) {
					std::vector<glm::vec2> corners = gs.player.cluster.getTriangleCorners(coords.first, coords.second);

					// is foodpos inside the triangle?
					if (GeoHelpers::pointInTriangle(foodpos, corners[0], corners[1], corners[2])) {
						toErase.push_back(i);
						// play sound
						Sound::play(*Player_Grow, sound_effect_volume*0.5f, 0.0f);

						// add a new triangle to the nearest side
						float d1 = GeoHelpers::pointToSegmentDistance(foodpos, corners[0], corners[1]);
						float d2 = GeoHelpers::pointToSegmentDistance(foodpos, corners[1], corners[2]);
						float d3 = GeoHelpers::pointToSegmentDistance(foodpos, corners[2], corners[0]);

						float minDist = fmin(d1, fmin(d2, d3));

						auto addTriangle = [&](int x, int y) {
							toInsert.push_back({{x, y}, PlayerTriangle(std::rand()%2 + 1)});
						};

						if (minDist == d1) {
							if (coords.first%2 == 0) {
								addTriangle(coords.first+1, coords.second-1);
							} else {
								addTriangle(coords.first-1, coords.second+1);
							}
						} else if (minDist == d2) {
							if (coords.first%2 == 0) {
								addTriangle(coords.first+1, coords.second);
							} else {
								addTriangle(coords.first-1, coords.second);
							}
						} else {
							if (coords.first%2 == 0) {
								addTriangle(coords.first-1, coords.second);
							} else {
								addTriangle(coords.first+1, coords.second);
							}
						}
						break;
					}
				}
			}
			for (int i = (int)toErase.size()-1; i >= 0; i--) {
				gs.food.erase(gs.food.begin() + toErase[i]);
			}
			for (auto k : toInsert) {
				std::pair<int,int> coords = k.first;
				PlayerTriangle t = k.second;
				if (!gs.player.cluster.triangles.count({coords.first, coords.second})) {
					gs.player.addTriangle(coords.first, coords.second, t);
				}
			}
		}
	}

	// update enemies and bullets
	{
		for (Enemy* e : gs.enemies) {
			e->update(elapsed, gs);
		}
		for (Bullet* b : gs.bullets) {
			b->update(elapsed, gs);
		}
	}

	//reset button press counters:
	{
		left.downs = 0;
		right.downs = 0;
		up.downs = 0;
		down.downs = 0;
		mouse.downs = 0;
		space.pressed = 0;
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	Drawer drawer(drawable_size);
	drawer.set_center(gs.player.cluster.pos);
	drawer.set_width(10.f);

	{ // draw the player
		for (std::pair<int,int> coords : gs.player.cluster.triangles) {
			std::vector<glm::vec2> corners = gs.player.cluster.getTriangleCorners(coords.first, coords.second);
			PlayerTriangle t = gs.player.triangle_info[coords];

			drawer.line(corners[0], corners[1], t.color[t.type]);
			drawer.line(corners[1], corners[2], t.color[t.type]);
			drawer.line(corners[2], corners[0], t.color[t.type]);
		}
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

	GL_ERRORS();
}
