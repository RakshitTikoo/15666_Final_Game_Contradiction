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
#include "Boss.hpp"
#include "GameState.hpp"

#include <random>
#include "Controls.hpp"

#include "DrawText.hpp"

using namespace std;

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
	
	// Entity Control
	enum Entity_Type {FOOD = 0, CHASER = 1, SHOOTER = 2, SPIRAL = 3, WORM = 4, INFECTOR = 5, BOMBER = 6, TROJAN = 7};
	void spawn_entity(int entity_type);
	void spawn_entities(int count, int entity_type);

	static constexpr int MAX_FOOD = 300;
	static constexpr int NUM_LEVELS = 1;
	vector<vector<pair<int, int>>> levels[NUM_LEVELS] = {
		{ // Level 0
			{{10, CHASER}},
			{{20, CHASER}, {3, SHOOTER}},
			{{40, CHASER}, {5, SHOOTER}, {2, SPIRAL}},
			{{50, CHASER}, {10, SHOOTER}, {5, SPIRAL}},
			{{1, TROJAN}}
		}
	};

	//----- game state -----
	//input tracking:
	Controls controls;

	GameState gs;

	DrawText TextRenderer;

};
