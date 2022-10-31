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
	} left, right, down, up, rot_left, rot_right, mouse;

	Player player;


	
	// Game Control Values
	float level_bound_x = 1000.0f;
	float level_bound_y = 1000.0f;

	float player_speed = 5.0f;
	float player_rot = 300.0f;
	float bullet_speed = 15.0f;

	float bullet_cooldown_cnt = 10.0f;
	float bullet_cooldown = 10.0f;

	float enemy_speed = 2.0f;

	int enemy_cnt = 10;
	int food_cnt = 20;

	float rad_basic_enemy = 0.25f;

	glm::vec2 mouse_loc;

	int triangle_type[10] = {1,1,1,1,1,1,2,2,2,2}; // temp, // 0 - basic , 1 - shooter

	// Music Samples
	std::shared_ptr< Sound::PlayingSample > MainLoop;
	float main_volume = 0.5f;
	float sound_effect_volume = 1.0f;

	std::vector<glm::vec2> food; // temporary
	std::vector<glm::vec2> enemy;
	std::vector<glm::vec2> player_bullet_pos;
	std::vector<glm::vec2> player_bullet_speed;
};
