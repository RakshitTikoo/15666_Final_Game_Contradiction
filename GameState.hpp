#pragma once

#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include <glm/glm.hpp>
#include <vector>
#include "Sound.hpp"
#include "data_path.hpp"
#include "Boss.hpp"
#include "DrawText.hpp"
#include "Builder.hpp"

struct GameState {
    GameState();

    enum State { Menu, Controls, Building, Playing };
    State state = Building;

    std::shared_ptr<Sound::PlayingSample> MainLoop;

    float sound_effect_volume = 1.f;
    float main_volume = 0.5f;

    glm::vec2 arena_max = {50.f, 50.f};
    glm::vec2 arena_min = {-50.f, -50.f};

    Player player;
    Trojan* trojan = nullptr;

    std::vector<Enemy *> enemies;
    std::vector<Bullet *> bullets;
    std::vector<glm::vec2> food;

    uint32_t score = 0;

    Sound::Sample* main_music;
    Sound::Sample* player_destroyed;
    Sound::Sample* player_grow;
    Sound::Sample* player_hit;
    Sound::Sample* player_bullet;

	int current_wave = 0;
    int current_level = 0;

    bool in_arena(glm::vec2 p);
};
