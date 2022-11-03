#include "Enemy.hpp"
#include "GameState.hpp"

Chaser::Chaser(glm::vec2 pos) {
    this->pos = pos;
}
void Chaser::draw(Drawer& drawer) {
    drawer.circle(this->pos, .25f, glm::uvec4(255.f, 0.f, 0.f, 255.f));
}
void Chaser::update(float elapsed, GameState& state) {
    // Walk towards player
    glm::vec2 dir = glm::normalize(state.player.cluster.pos - this->pos);
    this->pos += dir * elapsed * 4.f;

    // TODO: destroy stuff
}

Shooter::Shooter(glm::vec2 pos) {
    this->pos = pos;
}
void Shooter::draw(Drawer& drawer) {
    drawer.circle(this->pos, .25f, glm::uvec4(255.f, 177.f, 0.f, 255.f));
}
void Shooter::update(float elapsed, GameState& state) {
    // Walk towards player if too far
    glm::vec2 player_pos = state.player.cluster.pos;
    float dist = glm::length(player_pos - this->pos);
    if (dist >= 5.0f) {
        glm::vec2 dir = glm::normalize(player_pos - this->pos);
        this->pos += dir * elapsed * 2.f;
    }

    // Shoot bullet towards player
    if (bullet_cooldown <= 0.0f) {
        ShooterBullet* b = new ShooterBullet(this->pos, 15.f * glm::normalize(player_pos - this->pos));
        state.bullets.push_back(b);
    }

    // TODO: destroy stuff
}
