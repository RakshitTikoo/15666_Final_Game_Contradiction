#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <random>
#include <cmath>
#include <iostream>

#include "GeoHelpers.hpp"

#define WIDTH 960
#define HEIGHT 540

//compiles+links an OpenGL shader program from source.
// throws on compilation error.
GLuint gl_compile_program(std::string const &vertex_shader_source,std::string const &fragment_shader_source);

PlayMode::PlayMode() {
	player = Player();
	player.addTriangle(0, 0, PlayerTriangle());

	for (int i = 0; i < 50; i++) {
		glm::vec2 pos = {rand01() * 16 - 8, rand01() * 16 - 8};
		food.push_back(pos);
	}

	for (int i = 0; i < 20; i++) {
		glm::vec2 pos = {rand01() * 16 - 8, rand01() * 16 - 8};
		bullet.push_back(pos);
	}

	std::cout << "Initialize successful\n"; 
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_a) {
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
		} else if (evt.key.keysym.sym == SDLK_LEFT) {
			rot_left.downs += 1;
			rot_left.pressed = true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			rot_right.downs += 1;
			rot_right.pressed = true;
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
		} else if (evt.key.keysym.sym == SDLK_LEFT) {
			rot_left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			rot_right.pressed = false;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {
	//player movement:
	{
		//combine inputs into a move:
		constexpr float PlayerSpeed = 3.0f;
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed) move.x =-1.0f;
		if (!left.pressed && right.pressed) move.x = 1.0f;
		if (down.pressed && !up.pressed) move.y =-1.0f;
		if (!down.pressed && up.pressed) move.y = 1.0f;

		//make it so that moving diagonally doesn't go faster:
		if (move != glm::vec2(0.0f)) move = glm::normalize(move) * PlayerSpeed * elapsed;

		player.cluster.pos += move;

		if (rot_left.pressed) {
			player.cluster.angle -= 120.0f * elapsed;
			player.cluster.angle = std::fmodf(player.cluster.angle + 360.0f, 360.0f);
		}
		if (rot_right.pressed) {
			player.cluster.angle += 120.0f * elapsed;
			player.cluster.angle = std::fmodf(player.cluster.angle + 360.0f, 360.0f);
		}
	}
 
	// eat food = grow a triangle
	{
		std::vector<int> toErase;
		std::vector<std::pair<std::pair<int, int>, PlayerTriangle>> toInsert;
		for (int i = 0; i < (int)food.size(); i++) {
			glm::vec2 foodpos = food[i];
			for (std::pair<int,int> coords : player.cluster.triangles) {
				std::vector<glm::vec2> corners = player.cluster.getTriangleCorners(coords.first, coords.second);
				
				// is foodpos inside the triangle?
				if (GeoHelpers::pointInTriangle(foodpos, corners[0], corners[1], corners[2])) {
					toErase.push_back(i);

					// add a new triangle to the nearest side
					float d1 = GeoHelpers::pointToSegmentDistance(foodpos, corners[0], corners[1]);
					float d2 = GeoHelpers::pointToSegmentDistance(foodpos, corners[1], corners[2]);
					float d3 = GeoHelpers::pointToSegmentDistance(foodpos, corners[2], corners[0]);

					float minDist = fmin(d1, fmin(d2, d3));

					if (minDist == d1) {
						if (coords.first%2 == 0) {
							toInsert.push_back({{coords.first+1, coords.second-1}, PlayerTriangle()});
						} else {
							toInsert.push_back({{coords.first-1, coords.second+1}, PlayerTriangle()});
						}
					} else if (minDist == d2) {
						if (coords.first%2 == 0) {
							toInsert.push_back({{coords.first+1, coords.second}, PlayerTriangle()});
						} else {
							toInsert.push_back({{coords.first-1, coords.second}, PlayerTriangle()});
						}
					} else {
						if (coords.first%2 == 0) {
							toInsert.push_back({{coords.first-1, coords.second}, PlayerTriangle()});
						} else {
							toInsert.push_back({{coords.first+1, coords.second}, PlayerTriangle()});
						}
					}
					break;
				}
			}
		}
		for (int i = (int)toErase.size()-1; i >= 0; i--) {
			food.erase(food.begin() + toErase[i]);
		}
		for (auto k : toInsert) {
			std::pair<int,int> coords = k.first;
			PlayerTriangle t = k.second;
			if (!player.cluster.triangles.count({coords.first, coords.second})) {
				player.addTriangle(coords.first, coords.second, t);
			}
		}
	}

	// collision with bullets
	{
		std::vector<int> toErase_bullet;
		std::vector<std::pair<int, int>> toErase_player;
		for (int i = 0; i < (int)bullet.size(); i++) {
			glm::vec2 bulletpos = bullet[i];
			for (std::pair<int,int> coords : player.cluster.triangles) {
				std::vector<glm::vec2> corners = player.cluster.getTriangleCorners(coords.first, coords.second);
				
				// is foodpos inside the triangle?
				if (GeoHelpers::pointInTriangle(bulletpos, corners[0], corners[1], corners[2])) {
					toErase_bullet.push_back(i);
					toErase_player.push_back(coords);
					break;
				}
			}
		}
		for (int i = (int)toErase_bullet.size()-1; i >= 0; i--) {
			bullet.erase(bullet.begin() + toErase_bullet[i]);
		}
		player.destroyTriangles(toErase_player);
	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {

	static std::array< glm::vec2, 16 > const circle = [](){
		std::array< glm::vec2, 16 > ret;
		for (uint32_t a = 0; a < ret.size(); ++a) {
			float ang = a / float(ret.size()) * 2.0f * float(M_PI);
			ret[a] = glm::vec2(std::cos(ang), std::sin(ang));
		}
		return ret;
	}();

	// Not sure what these do but we can change when necessary
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	// TODO: pick values such that the camera follows the player and zooms out as the player gets larger
	float aspect = float(drawable_size.x) / float(drawable_size.y);
	float scale = 1.f / 10.f;
	glm::vec2 offset = {0.f, 0.f};
	glm::mat4 world_to_clip = glm::mat4(
		scale / aspect, 0.0f, 0.0f, offset.x,
		0.0f, scale, 0.0f, offset.y,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	DrawLines lines(world_to_clip);

	{ // draw the player
		for (std::pair<int,int> coords : player.cluster.triangles) {
			std::vector<glm::vec2> corners = player.cluster.getTriangleCorners(coords.first, coords.second);
			PlayerTriangle t = player.triangle_info[coords];

			lines.draw(
				glm::vec3(corners[0], 0.f),
				glm::vec3(corners[1], 0.f),
				t.color
			);
			lines.draw(
				glm::vec3(corners[1], 0.f),
				glm::vec3(corners[2], 0.f),
				t.color
			);
			lines.draw(
				glm::vec3(corners[2], 0.f),
				glm::vec3(corners[0], 0.f),
				t.color
			);
		}
	}

	{ // draw food
		for (auto& k : food) {
			float rad = 0.1f;
			for (uint32_t a = 0; a < circle.size(); ++a) {
				lines.draw(
					glm::vec3(k + rad * circle[a], 0.0f),
					glm::vec3(k + rad * circle[(a+1)%circle.size()], 0.0f),
					glm::u8vec4(0xff, 0xff, 0x00, 0xff)
				);
			}
		}
	}

	{ // draw bullets
		for (auto& k : bullet) {
			float rad = 0.1f;
			for (uint32_t a = 0; a < circle.size(); ++a) {
				lines.draw(
					glm::vec3(k + rad * circle[a], 0.0f),
					glm::vec3(k + rad * circle[(a+1)%circle.size()], 0.0f),
					glm::u8vec4(0x00, 0xff, 0xff, 0xff)
				);
			}
		}
	}

	GL_ERRORS();
}
