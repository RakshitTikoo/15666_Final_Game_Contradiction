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

#include <fstream>
#include <iostream>

#include "TriangleCluster.hpp"
#include "Player.hpp"
#include "Boss.hpp"
#include "GameState.hpp"
#include "Builder.hpp"

#include "rng.hpp"
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


	void init(int state); // 0 - new level or game over, 1 - level change 
	void destroy_entities();
	// Entity Control
	enum Entity_Type {FOOD = 0, CHASER = 1, SHOOTER = 2, SPIRAL = 3, WORM = 4, INFECTOR = 5, BOMBER = 6, TROJAN = 7, INFBOSS = 8, TIMESTOPBOSS = 9};
	void spawn_entity(int entity_type);
	void spawn_entities(int count, int entity_type);

	static constexpr int MAX_FOOD = 300;
	static constexpr int NUM_LEVELS = 3;
	vector<vector<pair<int, int>>> levels[NUM_LEVELS] = {
		{ // Level 0
			{{1, CHASER}},
			//{{2, CHASER}, {3, SHOOTER}},
			//{{4, CHASER}, {5, SHOOTER}, {2, SPIRAL}},
			//{{5, CHASER}, {1, SHOOTER}, {5, SPIRAL}},
			//{{1, TROJAN}}
		}, 
		{ // Level 1
			{{1, CHASER}, {3, SHOOTER}},
			//{{2, CHASER}, {3, SHOOTER}, {2, SPIRAL}},
			//{{4, CHASER}, {5, SHOOTER}, {5, SPIRAL}, {5, INFECTOR}},
			//{{5, CHASER}, {5, SHOOTER}, {5, SPIRAL}, {5, INFECTOR}},
			//{{1, INFBOSS}}
		}, 
		{ // Level 2
			{{1, CHASER}, {3, SHOOTER}, {2, SPIRAL}},
			//{{2, CHASER}, {3, SHOOTER}, {5, SPIRAL}, {5, INFECTOR}},
			//{{4, CHASER}, {5, SHOOTER}, {5, SPIRAL}, {5, INFECTOR}, {10, WORM}},
			//{{5, CHASER}, {1, SHOOTER}, {5, SPIRAL}, {5, INFECTOR}, {10, WORM}},
			//{{1, TIMESTOPBOSS}}
		}
	};

	pair<int, int> freemode[6] = {
		{10, CHASER},
		{5, SHOOTER},
		{-5, SPIRAL},
		{-10, INFECTOR},
		{-20, WORM},
		{-30, BOMBER}
	};

	int unlocked_levels = 0;
	std::string locked_msg = "";

	//----- game state -----
	//input tracking:
	Controls controls;

	GameState gs;

	Builder builder;

	DrawText TextRenderer;

	// Title screen options
	int selected_option = 0;
	const char* title_options[6] = {
		"Level 1", "Level 2", "Level 3", "Free Mode", "Controls", "Quit" 
	};

	float title_options_scale[6] = {
		0.75f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f
	};

	glm::vec3 title_options_color[6] = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f) 
	}; 

};
