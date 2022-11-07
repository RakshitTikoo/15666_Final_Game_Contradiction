#include "GameState.hpp"

GameState::GameState() {
    main_music = new Sound::Sample(data_path("Main_Music.opus"));
    player_destroyed = new Sound::Sample(data_path("player_destroyed.opus"));
    player_grow = new Sound::Sample(data_path("player_grow.opus"));
    player_hit = new Sound::Sample(data_path("player_hit.opus"));
    player_bullet = new Sound::Sample(data_path("player_bullet.opus"));
}

bool GameState::in_arena(glm::vec2 p) {
    return arena_min.x <= p.x && p.x <= arena_max.x && arena_min.y <= p.y && p.y <= arena_max.y;
}
