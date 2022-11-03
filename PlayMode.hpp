#pragma once

#include "Mode.hpp"

#include "Scene.hpp"
#include "WalkMesh.hpp"
#include "Sound.hpp"
#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <array>
#include <list>

#include <SDL.h>

#include "TriangleCluster.hpp"
#include "Player.hpp"

#include "GameState.hpp"

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

	//----- game state -----
	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, rot_left, rot_right, mouse, space;
	glm::vec2 mouse_loc;

	// Game Control Values
	int begin_game;
	float player_speed = 9.f;
	float player_rot = 300.f;

	// Music Samples
	std::shared_ptr< Sound::PlayingSample > MainLoop;
	float main_volume = 0.5f;
	float sound_effect_volume = 1.f;

	GameState gs;
};
