#pragma once

#include "Mode.hpp"

#include "Scene.hpp"
#include "WalkMesh.hpp"

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

	//----- game state -----
	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, rot_left, rot_right;

	Player player;

	std::vector<glm::vec2> food; // temporary
	std::vector<glm::vec2> bullet;
};
