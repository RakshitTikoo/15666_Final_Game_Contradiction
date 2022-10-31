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
	return new Sound::Sample(data_path("Main_Music.opus"));
});


void PlayMode::player_move(glm::vec2 move_amt){
	// Move everything by that negative amount to simulate player movement
	for (int i = 0; i < (int)food.size(); i++) {
			food[i] -= move_amt;
	}
	for (int i = 0; i < (int)enemy.size(); i++) {
			enemy[i] -= move_amt;
	}

	for (int i = 0; i < (int)player_bullet_pos.size(); i++) {
			player_bullet_pos[i] -= move_amt;
	}

	level_bound_max -= move_amt;
	level_bound_min -= move_amt;


}


PlayMode::PlayMode() {
	player = Player();
	player.addTriangle(0, 0, PlayerTriangle(), 0);
	//player.cluster

	for (int i = 0; i < food_cnt; i++) {
		float signx;
		float signy;
		if(rand()%2 == 0) signx = 1.0f;
		else signx = -1.0f;
		if(rand()%2 == 0) signy = 1.0f;
		else signy = -1.0f;
		glm::vec2 pos = {rand01() * signx * level_bound_max.x, rand01() * signy * level_bound_max.y};
		food.push_back(pos);
	}

	for (int i = 0; i < enemy_cnt; i++) {
		float signx;
		float signy;
		if(rand()%2 == 0) signx = 1.0f;
		else signx = -1.0f;
		if(rand()%2 == 0) signy = 1.0f;
		else signy = -1.0f;

		glm::vec2 pos = {rand01() * signx * level_bound_max.x, rand01() * signy * level_bound_max.y};
		enemy.push_back(pos);
	}

	std::cout << "Initialize successful\n"; 
	
	
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
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		mouse.downs += 1;
		mouse.pressed = true;
		return true;
	} else if (evt.type == SDL_MOUSEBUTTONUP) {
		mouse.pressed = false;
		return true;
	}
	int x, y;
	SDL_GetMouseState(&x, &y);
	mouse_loc = glm::vec2((float) x, (float) y);
	mouse_loc.x = mouse_loc.x - float(window_size.x)/2.0f;
	mouse_loc.y = float(window_size.y)/2.0f - mouse_loc.y;

	//printf("%f %f \n", mouse_loc.x , mouse_loc.y);

	return false;
}

void PlayMode::update(float elapsed) {
	// ===================
	// player movement
	// ===================
	{
		//combine inputs into a move:
		//constexpr float PlayerSpeed = 3.0f;
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed) move.x =-1.0f;
		if (!left.pressed && right.pressed) move.x = 1.0f;
		if (down.pressed && !up.pressed) move.y =-1.0f;
		if (!down.pressed && up.pressed) move.y = 1.0f;


		// =================
		// level bounds 
		// =================
	
		// Remove Bullet
		for (int i = 0; i < (int)player_bullet_pos.size(); i++) {
				if (player_bullet_pos[i].x > level_bound_max.x || player_bullet_pos[i].x < level_bound_min.x) {
					player_bullet_pos.erase(player_bullet_pos.begin() + i);
					player_bullet_speed.erase(player_bullet_speed.begin() + i);
				}
				else if (player_bullet_pos[i].y > level_bound_max.y || player_bullet_pos[i].y < level_bound_min.y) {
					player_bullet_pos.erase(player_bullet_pos.begin() + i);
					player_bullet_speed.erase(player_bullet_speed.begin() + i);
				}
		}

		// If Player at level_bound



		//make it so that moving diagonally doesn't go faster:
		if (move != glm::vec2(0.0f)) move = glm::normalize(move) * elapsed;

		glm::vec2 min_check = level_bound_min - player_speed*move;
		glm::vec2 max_check = level_bound_max - player_speed*move;
		if(min_check.x >= 0.0f || min_check.y >= 0.0f || max_check.x <= 0.0f || max_check.y <= 0.0f)
		{
			if(min_check.x >= 0.0f || max_check.x <= 0.0f) move.x = 0.0f;
			if(min_check.y >= 0.0f || max_check.y <= 0.0f) move.y = 0.0f;
		}

		//printf("min %f %f max %f %f\n", level_bound_min.x, level_bound_min.y, level_bound_max.x, level_bound_max.y );

		player_move(player_speed*move);
		//player.cluster.pos += player_speed * move;

		if (rot_left.pressed) {
			player.cluster.angle -= player_rot * elapsed;
			player.cluster.angle = std::fmodf(player.cluster.angle + 360.0f, 360.0f);
		}
		if (rot_right.pressed) {
			player.cluster.angle += player_rot * elapsed;
			player.cluster.angle = std::fmodf(player.cluster.angle + 360.0f, 360.0f);
		}
	}


	// ===============
	// enemy movement
	// ===============
	for (int i = 0; i < (int)enemy.size(); i++) {
			glm::vec2 dir = player.cluster.pos - enemy[i];
			dir = dir/glm::length(dir);
			enemy[i] += dir*elapsed*enemy_speed;
	}
	
 
	// ==============================
	// eat food = grow a triangle
	// ==============================
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
					// play sound
					Sound::play(*Player_Grow, sound_effect_volume, 0.0f);
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
				int rand_type = std::rand() % 10;
				player.addTriangle(coords.first, coords.second, t, triangle_type[rand_type]);
			}
		}
	}

	// ======================
	// collision with enemys
	// ======================
	{
		std::vector<int> toErase_enemy;
		std::vector<std::pair<int, int>> toErase_player;
		for (int i = 0; i < (int)enemy.size(); i++) {
			glm::vec2 enemypos = enemy[i];
			for (std::pair<int,int> coords : player.cluster.triangles) {
				std::vector<glm::vec2> corners = player.cluster.getTriangleCorners(coords.first, coords.second);
				
				// is enemy inside the triangle?
				if (GeoHelpers::pointInTriangle(enemypos, corners[0], corners[1], corners[2])) {
					// play sound
					Sound::play(*Player_Hit, sound_effect_volume*2.0f, 0.0f);

					toErase_enemy.push_back(i);
					toErase_player.push_back(coords);
					break;
				}
			}
		}
		for (int i = (int)toErase_enemy.size()-1; i >= 0; i--) {
			enemy.erase(enemy.begin() + toErase_enemy[i]);
		}
		player.destroyTriangles(toErase_player);
	}


	//TODO:
	// ==================================================
	// Spawn small amount of food randomly in play area
	// ==================================================
	if((int)food.size() <= food_cnt/2) {// 50 % food left
		for (int i = 0; i < food_cnt - (int)food.size(); i++) {
			float signx;
			float signy;
			if(rand()%2 == 0) signx = 1.0f;
			else signx = -1.0f;
			if(rand()%2 == 0) signy = 1.0f;
			else signy = -1.0f;
			glm::vec2 pos = {rand01() * signx * level_bound_max.x, rand01() * signy * level_bound_max.y};		
			food.push_back(pos);
		}
	}
	// =============================================
	// Spawn new enemies (if 50 % enemies defeated)
	// =============================================
	if((int)enemy.size() <= enemy_cnt/2) { // 50% enemies left
		for (int i = 0; i < enemy_cnt - (int)enemy.size(); i++) {
			float signx;
			float signy;
			if(rand()%2 == 0) signx = 1.0f;
			else signx = -1.0f;
			if(rand()%2 == 0) signy = 1.0f;
			else signy = -1.0f;

			glm::vec2 pos = {rand01() * signx * level_bound_max.x, rand01() * signy * level_bound_max.y};
			enemy.push_back(pos);
		}
	}


	// =================
	// game over logic
	// =================

	// =================
	// game win logic
	// =================


	// =================
	// player shooting 
	// =================
	bullet_cooldown_cnt += 1.0f;
	if(mouse.pressed && bullet_cooldown_cnt >= bullet_cooldown) {
		//printf("Shoot\n");
		bullet_cooldown_cnt = 0.0f;
		glm::vec2 dir = mouse_loc - player.cluster.pos;
		dir = dir/glm::length(dir);
		player_bullet_pos.push_back(player.cluster.pos);
		player_bullet_speed.push_back(dir);
	}

	// Bullet movement
	for(int i = 0; i < (int)player_bullet_pos.size(); i++) {
		player_bullet_pos[i] += player_bullet_speed[i]*elapsed*bullet_speed;
	}


	// =======================
	// bullet enemy collision 
	// =======================
	for(int i = 0; i < (int)player_bullet_pos.size(); i++){
		for(int j = 0; j < (int)enemy.size(); j++) {
			if(GeoHelpers::pointInCircle(player_bullet_pos[i], enemy[j], rad_basic_enemy)) {
				player_bullet_pos.erase(player_bullet_pos.begin() + i);
				player_bullet_speed.erase(player_bullet_speed.begin() + i);
				enemy.erase(enemy.begin() + j);
				break;
			}
		}
	}
	
	

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	mouse.downs = 0;

}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	
	//printf("%f %f\n", float(drawable_size.x), float(drawable_size.y));

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
				t.color[player.cluster.triangle_type[{coords.first, coords.second}]]
			);
			lines.draw(
				glm::vec3(corners[1], 0.f),
				glm::vec3(corners[2], 0.f),
				t.color[player.cluster.triangle_type[{coords.first, coords.second}]]
			);
			lines.draw(
				glm::vec3(corners[2], 0.f),
				glm::vec3(corners[0], 0.f),
				t.color[player.cluster.triangle_type[{coords.first, coords.second}]]
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
					glm::u8vec4(0x00, 0xff, 0x00, 0xff)
				);
			}
		}
	}

	{ // draw enemys
		for (auto& k : enemy) {
			float rad = rad_basic_enemy;
			for (uint32_t a = 0; a < circle.size(); ++a) {
				lines.draw(
					glm::vec3(k + rad * circle[a], 0.0f),
					glm::vec3(k + rad * circle[(a+1)%circle.size()], 0.0f),
					glm::u8vec4(0xff, 0x00, 0x00, 0xff)
				);
			}
		}
	}

	{ // draw player bullets
		for (auto& k : player_bullet_pos) {
			float rad = 0.1f;
			for (uint32_t a = 0; a < circle.size(); ++a) {
				lines.draw(
					glm::vec3(k + rad * circle[a], 0.0f),
					glm::vec3(k + rad * circle[(a+1)%circle.size()], 0.0f),
					glm::u8vec4(0xff, 0x00, 0xff, 0xff)
				);
			}
		}
	}

	// draw bounds
	{
		lines.draw(
				glm::vec3(level_bound_min.x, level_bound_min.y, 0.f),
				glm::vec3(level_bound_min.x, level_bound_max.y, 0.f),
				glm::u8vec4(0xff, 0xff, 0xff, 0xff)
			);
		lines.draw(
				glm::vec3(level_bound_min.x, level_bound_max.y, 0.f),
				glm::vec3(level_bound_max.x, level_bound_max.y, 0.f),
				glm::u8vec4(0xff, 0xff, 0xff, 0xff)
			);
		lines.draw(
				glm::vec3(level_bound_max.x, level_bound_max.y, 0.f),
				glm::vec3(level_bound_max.x, level_bound_min.y, 0.f),
				glm::u8vec4(0xff, 0xff, 0xff, 0xff)
			);
		lines.draw(
				glm::vec3(level_bound_max.x, level_bound_min.y, 0.f),
				glm::vec3(level_bound_min.x, level_bound_min.y, 0.f),
				glm::u8vec4(0xff, 0xff, 0xff, 0xff)
			);
		
	}

	GL_ERRORS();
}
