#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/norm.hpp>

#include <random>
#include <cmath>
#include <iostream>

#include "GeoHelpers.hpp"
#include "Drawer.hpp"

#define WIDTH 960
#define HEIGHT 540

//compiles+links an OpenGL shader program from source.
// throws on compilation error.
GLuint gl_compile_program(std::string const &vertex_shader_source,std::string const &fragment_shader_source);

void PlayMode::spawn_entity(int entity_type) {
	glm::vec2 pos = glm::vec2(rng::rand01(), rng::rand01()) * (gs.arena_max - gs.arena_min) + gs.arena_min;
	if (entity_type != FOOD) {
		// reroll until far enough away
		while (glm::length(pos - gs.player.cluster.pos) < 30) {
			pos = glm::vec2(rng::rand01(), rng::rand01()) * (gs.arena_max - gs.arena_min) + gs.arena_min;
		}
	}

	switch(entity_type) {
		case FOOD:
			gs.food.push_back(pos);
			break;

		case CHASER:
			gs.enemies.push_back(new Chaser(pos));
			break;
		
		case SHOOTER:
			gs.enemies.push_back(new Shooter(pos));
			break;

		case SPIRAL:
			gs.enemies.push_back(new Spiral(pos));
			break;
		
		case BOMBER:
			gs.enemies.push_back(new Bomber(pos));
			break;

		case INFECTOR:
			gs.enemies.push_back(new Infector(pos));
			break;
		
		case WORM:
			gs.enemies.push_back(new WormSegment(pos, nullptr));
			break;
		
		case TROJAN:
			gs.trojan = new Trojan(pos);
			break;

		case INFBOSS:
			gs.infboss = new Infboss(pos);
			break;

		case TIMESTOPBOSS:
			gs.timestopboss = new Timestopboss(pos);
			break;
			

		default: 
			printf("\nError: Unknown Entity Type\n");
			break;
	}
}

void PlayMode::spawn_entities(int count, int entity_type) {
	for (int i = 0; i < count; i++) {
		spawn_entity(entity_type);
	}
}

void PlayMode::destroy_entities()
{

	gs.bullets.clear();
	gs.food.clear();
	gs.enemies.clear();

	// Clear bosses
	if(gs.trojan != nullptr) {
		for (auto& k : gs.trojan->triangle_info) {
            gs.trojan->eraseSingleTriangle(k.first.first, k.first.second);
        }
		gs.trojan = nullptr;
	}

	if(gs.infboss != nullptr) {
		for (auto& k : gs.infboss->triangle_info) {
            gs.infboss->eraseSingleTriangle(k.first.first, k.first.second);
        }
		gs.infboss = nullptr;
	}

	if(gs.timestopboss != nullptr) {
		for (auto& k : gs.timestopboss->triangle_info) {
            gs.timestopboss->eraseSingleTriangle(k.first.first, k.first.second);
        }
		gs.timestopboss = nullptr;
	}


	// Destroy player

	for (auto& k : gs.player.triangle_info) {
            gs.player.eraseSingleTriangle(k.first.first, k.first.second);
	}


}
void PlayMode::init(int state){
	destroy_entities();

	gs.player = Player();

	if(state == 0) {
		gs.score = 0;
		gs.current_level = 0;
		gs.current_wave = -1; // hacky way to get first wave to spawn on update
		gs.state = gs.Menu;
	}
	else  {
		gs.state = gs.Building; // Go to building
		builder.remaining_money = gs.money;
	}

	
	
}

PlayMode::PlayMode() {
	this->TextRenderer = DrawText("NotoSansMono_Condensed-Regular.ttf");

	init(0);

	// Read save file
	ifstream a_file ("Game.save");
	if (a_file.is_open()) {
		std::string level;
		a_file >> level;
		unlocked_levels = std::stoi(level);
		printf("11 %d", unlocked_levels);
  	}



	std::cout << "Initialization successful\n"; 
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	if (gs.state == gs.Building) {
		return builder.handle_event(evt, window_size);
	}

	gs.window_min = {0.f, 0.f};
	gs.window_max = window_size;
	return controls.handle_event(evt, window_size);
}

void PlayMode::update(float elapsed) {
	if (gs.state == gs.Menu) { 
		// =========================
		// Title Screen Settings
		// =========================

		if(controls.arrow_up.pressed && controls.arrow_up.once == 1) {
			controls.arrow_up.once = 2;
			selected_option -= 1;
			locked_msg = "";
		}
		
		if(controls.arrow_down.pressed && controls.arrow_down.once == 1) {
			controls.arrow_down.once = 2;
			selected_option += 1;
			locked_msg = "";
		}

		if(selected_option < 0) selected_option = 0;
		if(selected_option > 5) selected_option = 5;


		if (gs.MainLoop != nullptr){
			gs.MainLoop->volume = 0.f;
		}
		for(int i = 0; i < 6; i++) {
			if(i != selected_option) {
				title_options_color[i] = glm::vec3(0.5f, 0.5f, 0.5f);
				title_options_scale[i] = 0.5f;
			} else {
				title_options_color[i] = glm::vec3(1.0f, 1.0f, 1.0f);
				title_options_scale[i] = 0.75f;
			}
		}
		if(controls.enter.pressed) {
			if(selected_option == 0) { // Level 1 select
				gs.money = 500;
				gs.state = gs.Building;
				gs.current_level = 0;
				builder = Builder(gs.money);
			}
			if(selected_option == 1) { // Level 2 select
				if(unlocked_levels > 0) {
					gs.money = 500;
					gs.state = gs.Building;
					gs.current_level = 1; 
					builder = Builder(gs.money);
				}
				else {
					locked_msg = "Level Locked";
				}
			}
			if(selected_option == 2) { // Level 3 select
				if(unlocked_levels > 1) {
					gs.money = 500;
					gs.state = gs.Building;
					gs.current_level = 2;
					builder = Builder(gs.money);
				}
				else {
					locked_msg = "Level Locked";
				}
			}
			if(selected_option == 3) { // Free Mode select
				if(unlocked_levels > 2) {
					gs.money = 2000;
					gs.state = gs.Building;
					gs.current_level = 3;
					builder = Builder(gs.money);
				}
				else {
					locked_msg = "Level Locked";
				}
			}

			if(selected_option == 4) { // Controls select
				gs.state = gs.Controls;
			}

			if(selected_option == 5) { // Quit select
				exit(0);
			}

		}
		return;
	}

	if (gs.state == gs.Controls) { // Control menu
		if(controls.escape.pressed) gs.state = gs.Menu;
		return;
	}

	if (gs.state == gs.Building) {
		controls.mouse.once = 2; //debounce
		// returns {remaining money, player} if done, otherwise the first number is -1
		pair<int, Player> result = builder.update(elapsed);
		if (result.first != -1) {
			if(gs.current_level == 0) {
				gs.state = gs.Level1;
				gs.MainLoop = Sound::loop(*gs.main_music, gs.main_volume, 0.0f);
			}
			if(gs.current_level == 1) {
				gs.state = gs.Level2;
				//gs.MainLoop = Sound::loop(*gs.main_music, gs.main_volume, 0.0f); // Can change song
			}
			if(gs.current_level == 2) {
				gs.state = gs.Level3;
				//gs.MainLoop = Sound::loop(*gs.main_music, gs.main_volume, 0.0f); // Can change song
			}
			if(gs.current_level == 3) {
				gs.state = gs.Level3;
				//gs.MainLoop = Sound::loop(*gs.main_music, gs.main_volume, 0.0f); // Can change song
			}
			if(gs.current_level == 4) {
				gs.state = gs.FreeMode;
				//gs.MainLoop = Sound::loop(*gs.main_music, gs.main_volume, 0.0f); // Can change song
			}
			
			gs.money = result.first;
			gs.player = result.second;
		}
		return;
	}


	if(gs.state == gs.Level1 || gs.state == gs.Level2 || gs.state == gs.Level3) {
		

		{ // Update wave
			if (gs.enemies.empty() && gs.trojan == nullptr && gs.infboss == nullptr && gs.timestopboss == nullptr) {
				gs.current_wave++;
				
				if (gs.current_wave == levels[gs.current_level].size()) { // Level Update
					
					gs.current_wave = -1;
					if(unlocked_levels < 4)
					{
						unlocked_levels += 1;
						gs.current_level++;
					}

					// Add save file
					ofstream a_file ("Game.save", ios::trunc);
					a_file<<std::to_string(unlocked_levels);
  					a_file.close();

					if (gs.current_level == NUM_LEVELS) { // Game end
						init(0);
						return;
					} else {
						init(1);
						return;
					}

				}
				
				// Spawn wave
				spawn_entities(MAX_FOOD - (int)gs.food.size(), FOOD);
				for (pair<int, int> to_spawn : levels[gs.current_level][gs.current_wave]) {
					spawn_entities(to_spawn.first, to_spawn.second);
				}
			}
		}

		{ // update player
			gs.player.update(elapsed, gs, controls);
		}

		{ // update boss
			if (gs.trojan != nullptr) {
				gs.trojan->update(elapsed, gs);
			}
		}

		{ // update boss
			if (gs.infboss != nullptr) {
				gs.infboss->update(elapsed, gs);
			}
		}

		{ // update boss
			if (gs.timestopboss != nullptr) {
				gs.timestopboss->update(elapsed, gs);
			}
		}


		{ // update enemies
			for (Enemy* e : gs.enemies) {
				e->update(elapsed, gs);
			}
		}

		{ // update bullets
			for (Bullet* b : gs.bullets) {
				b->update(elapsed, gs);
			}
		}

		{ // destroy dead things
			for (int i = (int)gs.enemies.size()-1; i >= 0; i--) {
				if (gs.enemies[i]->destroyed) {
					gs.enemies.erase(gs.enemies.begin() + i);
					gs.score += 1;
				}
			}
			for (int i = (int)gs.bullets.size()-1; i >= 0; i--) {
				if (gs.bullets[i]->destroyed) {
					gs.bullets.erase(gs.bullets.begin() + i);
				}
			}
			if (gs.player.cluster.triangles.size() == 0) {
				Sound::play(*gs.player_destroyed, 3.0f*gs.sound_effect_volume, 0.0f);
				init(0);
			}
		}

		
	}

	//reset button press counters:
	{
		controls.left.downs = 0;
		controls.right.downs = 0;
		controls.up.downs = 0;
		controls.down.downs = 0;
		controls.q.downs = 0;
		controls.e.downs = 0;
		controls.mouse.downs = 0;
		controls.space.pressed = 0;
		controls.enter.pressed = 0;
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gs.state == gs.Building) {
		builder.draw(drawable_size);
		GL_ERRORS();
		return;
	}

	Drawer drawer(drawable_size, TextRenderer);
	if (gs.state == gs.Menu) {
		drawer.text("Poly Defense", {100.f, 450.f}, 1.f);
		drawer.text(title_options[0], {100.f, 380.f}, title_options_scale[0], title_options_color[0]);
		drawer.text(title_options[1], {100.f, 330.f}, title_options_scale[1], title_options_color[1]);
		drawer.text(title_options[2], {100.f, 280.f}, title_options_scale[2], title_options_color[2]);
		drawer.text(title_options[3], {100.f, 230.f}, title_options_scale[3], title_options_color[3]);
		drawer.text(title_options[4], {100.f, 180.f}, title_options_scale[4], title_options_color[4]);
		drawer.text(title_options[5], {100.f, 130.f}, title_options_scale[5], title_options_color[5]);

		drawer.text(locked_msg, {600.f, 330.f}, 0.75f , glm::vec3(1.0f, 1.0f, 1.0f));
		
	} 
	else if(gs.state == gs.Controls) { // Controls
		drawer.text("You are an antivirus software, fighting through hordes of\n"
					 "viruses. Keep on building your antivirus software by combining\n"
					 "different power triangles at the beginning of each level.",
					{50.f, 500.f},
					0.5f,
					glm::vec3(1.0f, 1.0f, 1.0f)
		);

		drawer.text("WASD - Move\n"
					"Q - Rotate left\n"
					"E - Rotate right\n"
					"Space - Bomb attack\n"
					"F - Timestop attack\n",
					{50.f, 400.f},
					0.5f,
					glm::vec3(1.0f, 1.0f, 1.0f)
		);
	}
	else if(gs.state == gs.Level1 || gs.state == gs.Level2 || gs.state == gs.Level3) {
		drawer.text("$" + to_string(gs.money), {10.f, gs.window_max.y-30.f}, 0.4f);
		drawer.text_align_right("Wave " + std::to_string(gs.current_wave + 1), {950.f, gs.window_max.y-30.f}, 0.4f);
		drawer.text_align_right("Level " + std::to_string(gs.current_level + 1), {850.f, gs.window_max.y-30.f}, 0.4f);
		
		drawer.set_center(gs.player.cluster.pos);
		drawer.set_width(40.f);
		
		{ // draw the player
			gs.player.draw(drawer);
		}

		GL_ERRORS();
		
		{ // draw boss
			if (gs.trojan != nullptr) {
				gs.trojan->draw(drawer);
			}
		}

		GL_ERRORS();

		{ // draw boss
			if (gs.infboss != nullptr) {
				gs.infboss->draw(drawer);
			}
		}

		GL_ERRORS();

		
		{ // draw boss
			if (gs.timestopboss != nullptr) {
				gs.timestopboss->draw(drawer);
				gs.timestopboss->draw_timestop(drawer);
			}
		}

		GL_ERRORS();

		
		{ // draw player explosion
			gs.player.draw_explosion(drawer);
		
		}

		GL_ERRORS();

		{ // draw player timestop
			gs.player.draw_timestop(drawer);
		
		}

		GL_ERRORS();
		
		{ // draw food
			for (auto& k : gs.food) {
				drawer.circle(k, 0.1f, glm::u8vec4(0x00, 0xff, 0x00, 0xff));
			}
		}

		GL_ERRORS();
		
		{ // draw basic_enemys
			for (Enemy* e : gs.enemies) {
				e->draw(drawer);
			}
		}

		GL_ERRORS();
		
		{ // draw bullets
			for (Bullet* b : gs.bullets) {
				b->draw(drawer);
			}
		}

		GL_ERRORS();
		
		{ // draw bounds
			glm::u8vec4 color = {255.f, 255.f, 255.f, 255.f};
			drawer.line(glm::vec2(gs.arena_min.x, gs.arena_min.y),
						glm::vec2(gs.arena_min.x, gs.arena_max.y),
						color);
			drawer.line(glm::vec2(gs.arena_min.x, gs.arena_max.y),
						glm::vec2(gs.arena_max.x, gs.arena_max.y),
						color);
			drawer.line(glm::vec2(gs.arena_max.x, gs.arena_max.y),
						glm::vec2(gs.arena_max.x, gs.arena_min.y),
						color);
			drawer.line(glm::vec2(gs.arena_max.x, gs.arena_min.y),
						glm::vec2(gs.arena_min.x, gs.arena_min.y),
						color);
		}

		GL_ERRORS();
		
	}

	gs.drawer_min = drawer.center - glm::vec2(drawer.width/2.f, (drawer.width/2.f)/drawer.aspect);
	gs.drawer_max = drawer.center + glm::vec2(drawer.width/2.f, (drawer.width/2.f)/drawer.aspect);
	
	GL_ERRORS();
}
