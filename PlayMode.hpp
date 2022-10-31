#pragma once

#include "Mode.hpp"

#include "Scene.hpp"
#include "WalkMesh.hpp"
#include "Sound.hpp"
#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <array>

#include <SDL.h>

#include "TriangleCluster.hpp"
#include "Player.hpp"

#include <random>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	void init(int state);

	std::mt19937 mt; //rng stuff
	float rand01() {
		static float mx = (float)mt.max();
		return mt() / mx;
	}

	void player_move(glm::vec2 move_amt);

	//----- game state -----
	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, rot_left, rot_right, mouse, space;

	Player player;



	// Game Control Values
	int begin_game;
	
	glm::vec2 level_bound_max;
	glm::vec2 level_bound_min;

	float player_speed;
	float player_rot;
	float bullet_speed;

	float bullet_cooldown_cnt;
	float bullet_cooldown;

	float enemy_speed;

	int enemy_cnt;
	int food_cnt;

	float rad_basic_enemy;

	uint32_t score;

	glm::vec2 mouse_loc;

	std::string msg;
	int triangle_type[10] = {1,1,1,1,1,1,2,2,2,2}; // temp, // 0 - basic , 1 - shooter

	


	// Music Samples
	std::shared_ptr< Sound::PlayingSample > MainLoop;
	float main_volume;
	float sound_effect_volume;



	std::vector<glm::vec2> food; // temporary
	std::vector<glm::vec2> enemy;
	std::vector<glm::vec2> player_bullet_pos;
	std::vector<glm::vec2> player_bullet_speed;
};
