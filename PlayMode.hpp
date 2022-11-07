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
#include "Controls.hpp"

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	void init();

	std::mt19937 mt; //rng stuff
	float rand01() {
		static float mx = (float)mt.max();
		return mt() / mx;
	}

	//----- game state -----
	//input tracking:
	Controls controls;

	GameState gs;
};
