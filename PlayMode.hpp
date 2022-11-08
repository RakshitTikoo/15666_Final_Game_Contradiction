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
	
	enum Entity_Type {FOOD = 0, CHASER = 1, SHOOTER = 2, SPIRAL = 3, WORM = 4, INFECTOR = 5, BOMBER = 6};

	void spawn_entity(int count, int entity_type);
	void update_wave();

	




	// Wave Logic
	// Score 0 - 50 - Spawn Chasers 
	// Score 50 - 150 - Spawn Chasers + Shooters
	// Score 150 - 500 - Spawn Chasers + Shooters + Spirals 
	// Spawn 25 enemies of each type, and as soon as 50 % defeated spawn more
	// Spawn more food after each wave 




	//----- game state -----
	//input tracking:
	Controls controls;

	GameState gs;
};
